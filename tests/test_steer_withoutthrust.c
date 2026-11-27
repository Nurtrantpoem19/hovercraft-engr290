#define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include "adc.h"
#include "TWI_290.h"
#include "imu.h"
#include "sensors.h"
#include "servo.h"
#include "uart.h"

#define IR_CLOSE_THRESHOLD 165U
#define IR_FAR_WALL_MAX 80U
#define FALLBACK_MAX_IR 150U
#define LEG2_CORNER_FALLBACK_MAX 120U
#define LEG2_STRAIGHTEN_MAX 90U
#define SERVO_SETTLE_MS 250
#define SERVO_EDGE_STEP_DEGREES 10U
#define WALL_SAMPLES 5U
#define WALL_SAMPLE_DELAY_MS 5
#define SWEEP_SAMPLES 8U
#define SWEEP_SAMPLE_DELAY_MS 10

static volatile uint32_t system_millis;
static uint32_t last_imu_update_ms;
static uint8_t imu_error_reported;
static uint8_t far_wall_locked;
static float far_wall_bearing;

typedef enum {
    TRACK_STAGE_LEG1 = 0,
    TRACK_STAGE_CORNER1,
    TRACK_STAGE_LEG2
} TrackStage;

static TrackStage current_stage = TRACK_STAGE_LEG1;
static uint8_t current_angle = 90U;

ISR(TIMER2_COMPA_vect) {
    system_millis++;
}

static uint32_t millis(void) {
    uint32_t current_millis;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        current_millis = system_millis;
    }

    return current_millis;
}

static void timer2_init(void) {
    TCCR2A = (1 << WGM21);
    TCCR2B = (1 << CS22);
    OCR2A = 249U;
    TIMSK2 = (1 << OCIE2A);
    sei();
    last_imu_update_ms = millis();
}

static void imu_update_if_due(void) {
    uint32_t now = millis();
    uint32_t elapsed_ms = now - last_imu_update_ms;

    if (elapsed_ms < 20U) {
        return;
    }
    last_imu_update_ms = now;

    if (imu_read() != 0U) {
        if (!imu_error_reported) {
            uart_transmit_string("IMU read failed; yaw tracking paused\r\n");
            imu_error_reported = 1U;
        }
        return;
    }

    imu_error_reported = 0U;
    convert_accel_raw();
    convert_gyro_raw();
    rotation_update((float)elapsed_ms / 1000.0f);
}

static void delay_with_imu(uint16_t duration_ms) {
    while (duration_ms > 0U) {
        _delay_ms(1);
        duration_ms--;
        imu_update_if_due();
    }
}

static const char *stage_to_str(TrackStage stage) {
    switch (stage) {
        case TRACK_STAGE_LEG1:
            return "LEG1";
        case TRACK_STAGE_CORNER1:
            return "CORNER1";
        case TRACK_STAGE_LEG2:
            return "LEG2";
        default:
            return "UNKNOWN";
    }
}

static void set_stage(TrackStage next_stage) {
    if (current_stage != next_stage) {
        current_stage = next_stage;
        uart_transmit_string("Stage -> ");
        uart_transmit_string(stage_to_str(current_stage));
        uart_transmit_string("\r\n");
    }
}

static void set_servo_tracked(uint8_t angle) {
    if (angle > 180U) {
        angle = 180U;
    }
    current_angle = angle;
    servo_setAngle(current_angle);
}

static float normalize_degrees(float angle) {
    while (angle > 180.0f) {
        angle -= 360.0f;
    }
    while (angle < -180.0f) {
        angle += 360.0f;
    }
    return angle;
}

static void lock_far_wall_bearing(void) {
    far_wall_bearing = normalize_degrees(
        rotation.yaw + (float)((int16_t)current_angle - 90)
    );
    far_wall_locked = 1U;

    uart_transmit_string("Far-wall bearing locked; servo angle: ");
    uart_transmit_uint16(current_angle);
    uart_transmit_string(" deg\r\n");
}

static void track_far_wall_with_yaw(void) {
    if (!far_wall_locked) {
        return;
    }

    float heading_error = normalize_degrees(far_wall_bearing - rotation.yaw);
    int16_t desired_angle = (int16_t)(90.0f + heading_error
                                      + (heading_error >= 0.0f ? 0.5f : -0.5f));
    if (desired_angle < 0) {
        desired_angle = 0;
    } else if (desired_angle > 180) {
        desired_angle = 180;
    }

    uint8_t target_angle = (uint8_t)desired_angle;
    int16_t angle_difference = (int16_t)target_angle - current_angle;
    if (angle_difference >= 2 || angle_difference <= -2) {
        set_servo_tracked(target_angle);
        uart_transmit_string("Yaw ");
        if (rotation.yaw < 0.0f) {
            uart_transmit_char('-');
            uart_transmit_uint16((uint16_t)(-rotation.yaw));
        } else {
            uart_transmit_uint16((uint16_t)rotation.yaw);
        }
        uart_transmit_string(" deg; servo corrected to ");
        uart_transmit_uint16(current_angle);
        uart_transmit_string(" deg\r\n");
    }
}

static void straighten_toward_center(void) {
    far_wall_locked = 0U;

    if (current_angle > 90U) {
        uint8_t next = (current_angle - 90U > SERVO_EDGE_STEP_DEGREES)
                           ? (uint8_t)(current_angle - SERVO_EDGE_STEP_DEGREES)
                           : 90U;
        set_servo_tracked(next);
        delay_with_imu(SERVO_SETTLE_MS);
    } else if (current_angle < 90U) {
        uint8_t next = ((uint16_t)(90U - current_angle) > SERVO_EDGE_STEP_DEGREES)
                           ? (uint8_t)(current_angle + SERVO_EDGE_STEP_DEGREES)
                           : 90U;
        set_servo_tracked(next);
        delay_with_imu(SERVO_SETTLE_MS);
    }

    if (current_angle == 90U) {
        lock_far_wall_bearing();
    }
}

static uint16_t read_ir_average(void) {
    uint32_t total = 0;

    for (uint8_t sample = 0; sample < WALL_SAMPLES; sample++) {
        total += sensorsIR_read();
        delay_with_imu(WALL_SAMPLE_DELAY_MS);
        imu_update_if_due();
    }

    return (uint16_t)(total / WALL_SAMPLES);
}

static uint16_t read_sweep_average(void) {
    uint32_t total = 0;

    for (uint8_t sample = 0; sample < SWEEP_SAMPLES; sample++) {
        total += sensorsIR_read();
        delay_with_imu(SWEEP_SAMPLE_DELAY_MS);
        imu_update_if_due();
    }

    return (uint16_t)(total / SWEEP_SAMPLES);
}

static void edge_left_for_far_wall(void) {
    uint16_t lowest_reading = 1023U;
    uint8_t best_fallback_angle = 90U;
    uint8_t found_fallback = 0U;
    far_wall_locked = 0U;

    if (current_angle == 0U) {
        uint8_t relief_angle = (uint8_t)(0U + SERVO_EDGE_STEP_DEGREES);
        set_servo_tracked(relief_angle);
        delay_with_imu(SERVO_SETTLE_MS);
        uart_transmit_string("[");
        uart_transmit_string(stage_to_str(current_stage));
        uart_transmit_string("] At 0 deg left limit; stepping right to ");
        uart_transmit_uint16(current_angle);
        uart_transmit_string(" deg\r\n");
        return;
    }

    while (1) {
        uint8_t next_angle = (current_angle > SERVO_EDGE_STEP_DEGREES)
                                 ? (uint8_t)(current_angle - SERVO_EDGE_STEP_DEGREES)
                                 : 0U;

        set_servo_tracked(next_angle);
        delay_with_imu(SERVO_SETTLE_MS);

        uint16_t reading = read_sweep_average();
        uart_transmit_string("[");
        uart_transmit_string(stage_to_str(current_stage));
        uart_transmit_string("] Edge left angle: ");
        uart_transmit_uint16(current_angle);
        uart_transmit_string(" deg, IR: ");
        uart_transmit_uint16(reading);
        uart_transmit_string("\r\n");

        if (reading < lowest_reading) {
            lowest_reading = reading;
            if (reading <= FALLBACK_MAX_IR) {
                best_fallback_angle = current_angle;
                found_fallback = 1U;
            }
        }

        if (reading <= IR_FAR_WALL_MAX) {
            uart_transmit_string("[");
            uart_transmit_string(stage_to_str(current_stage));
            uart_transmit_string("] Locked onto far wall at: ");
            uart_transmit_uint16(current_angle);
            uart_transmit_string(" deg, IR: ");
            uart_transmit_uint16(reading);
            uart_transmit_string("\r\n");
            lock_far_wall_bearing();
            return;
        }

        if (found_fallback && lowest_reading < 140U && reading > (uint16_t)(lowest_reading + 80U)) {
            set_servo_tracked(best_fallback_angle);
            uart_transmit_string("[");
            uart_transmit_string(stage_to_str(current_stage));
            uart_transmit_string("] Spiked left; snapping to fallback ");
            uart_transmit_uint16(best_fallback_angle);
            uart_transmit_string(" deg (IR: ");
            uart_transmit_uint16(lowest_reading);
            uart_transmit_string(")\r\n");
            lock_far_wall_bearing();
            return;
        }

        if (current_angle == 0U) {
            break;
        }
    }

    if (found_fallback) {
        set_servo_tracked(best_fallback_angle);
        uart_transmit_string("[");
        uart_transmit_string(stage_to_str(current_stage));
        uart_transmit_string("] Limited fallback to angle: ");
        uart_transmit_uint16(best_fallback_angle);
        uart_transmit_string(" deg, IR: ");
        uart_transmit_uint16(lowest_reading);
        uart_transmit_string("\r\n");
        lock_far_wall_bearing();
    } else {
        set_servo_tracked(90U);
        delay_with_imu(SERVO_SETTLE_MS);
        uart_transmit_string("[");
        uart_transmit_string(stage_to_str(current_stage));
        uart_transmit_string("] No fallback <= ");
        uart_transmit_uint16(FALLBACK_MAX_IR);
        uart_transmit_string(" (lowest ");
        uart_transmit_uint16(lowest_reading);
        uart_transmit_string("); centering at 90 deg\r\n");
    }
}

static void edge_right_for_corner1(void) {
    uint16_t lowest_reading = 1023U;
    uint8_t best_fallback_angle = 90U;
    uint8_t found_fallback = 0U;
    far_wall_locked = 0U;

    if (current_angle >= 180U) {
        uint8_t relief_angle = (uint8_t)(180U - SERVO_EDGE_STEP_DEGREES);
        set_servo_tracked(relief_angle);
        delay_with_imu(SERVO_SETTLE_MS);
        uart_transmit_string("[CORNER1] At 180 deg right limit; stepping left to ");
        uart_transmit_uint16(current_angle);
        uart_transmit_string(" deg\r\n");
        return;
    }

    while (1) {
        uint8_t next_angle = ((uint16_t)current_angle + SERVO_EDGE_STEP_DEGREES < 180U)
                                 ? (uint8_t)(current_angle + SERVO_EDGE_STEP_DEGREES)
                                 : 180U;

        set_servo_tracked(next_angle);
        delay_with_imu(SERVO_SETTLE_MS);

        uint16_t reading = read_sweep_average();
        uart_transmit_string("[CORNER1] Edge right angle: ");
        uart_transmit_uint16(current_angle);
        uart_transmit_string(" deg, IR: ");
        uart_transmit_uint16(reading);
        uart_transmit_string("\r\n");

        if (reading < lowest_reading) {
            lowest_reading = reading;
            if (reading <= FALLBACK_MAX_IR) {
                best_fallback_angle = current_angle;
                found_fallback = 1U;
            }
        }

        if (reading <= IR_FAR_WALL_MAX) {
            uart_transmit_string("[CORNER1] Found opening at: ");
            uart_transmit_uint16(current_angle);
            uart_transmit_string(" deg, IR: ");
            uart_transmit_uint16(reading);
            uart_transmit_string("\r\n");
            set_stage(TRACK_STAGE_LEG2);
            lock_far_wall_bearing();
            return;
        }

        if (current_angle >= 180U) {
            break;
        }
    }

    if (found_fallback) {
        set_servo_tracked(best_fallback_angle);
        uart_transmit_string("[CORNER1] Limited fallback to angle: ");
        uart_transmit_uint16(best_fallback_angle);
        uart_transmit_string(" deg, IR: ");
        uart_transmit_uint16(lowest_reading);
        uart_transmit_string("\r\n");

        if (lowest_reading <= LEG2_CORNER_FALLBACK_MAX) {
            set_stage(TRACK_STAGE_LEG2);
            lock_far_wall_bearing();
        }
    } else {
        set_servo_tracked(90U);
        delay_with_imu(SERVO_SETTLE_MS);
        uart_transmit_string("[CORNER1] No fallback <= ");
        uart_transmit_uint16(FALLBACK_MAX_IR);
        uart_transmit_string(" (lowest ");
        uart_transmit_uint16(lowest_reading);
        uart_transmit_string("); centering at 90 deg\r\n");
    }
}

int main(void) {
    uart_init(F_CPU, 9600);
    adc_init();
    TWI_init();
    uint8_t imu_status = imu_init();
    if (imu_status != 0U) {
        uart_transmit_string("IMU initialization failed, code ");
        uart_transmit_uint16(imu_status);
        uart_transmit_string("\r\n");
        while (1) {
            _delay_ms(1000);
        }
    }

    timer2_init();
    servo_init();
    servo_center();
    current_angle = 90U;
    delay_with_imu(SERVO_SETTLE_MS);
    uart_transmit_string("IR + IMU yaw steering test without thrust; UART 9600 baud\r\n");
    uart_transmit_string("Stage -> LEG1\r\n");

    while (1) {
        imu_update_if_due();
        track_far_wall_with_yaw();

        uint16_t reading = read_ir_average();
        uart_transmit_string("[");
        uart_transmit_string(stage_to_str(current_stage));
        uart_transmit_string("] IR: ");
        uart_transmit_uint16(reading);
        uart_transmit_string("\r\n");

        if (current_stage == TRACK_STAGE_CORNER1 && reading <= IR_FAR_WALL_MAX) {
            set_stage(TRACK_STAGE_LEG2);
            straighten_toward_center();
        } else if (current_stage == TRACK_STAGE_LEG2 && reading <= LEG2_STRAIGHTEN_MAX && current_angle != 90U) {
            straighten_toward_center();
        }

        if (reading >= IR_CLOSE_THRESHOLD) {
            if (current_stage == TRACK_STAGE_LEG1) {
                if (current_angle <= 20U || reading >= 250U) {
                    set_stage(TRACK_STAGE_CORNER1);
                    edge_right_for_corner1();
                } else {
                    edge_left_for_far_wall();
                }
            } else if (current_stage == TRACK_STAGE_CORNER1) {
                edge_right_for_corner1();
            } else if (current_stage == TRACK_STAGE_LEG2) {
                edge_left_for_far_wall();
            }
        }

        delay_with_imu(50U);
    }
}
