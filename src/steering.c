#include "steering.h"
#include "sensors.h"
#include "servo.h"
#include <util/delay.h>

#define SERVO_RIGHT_US 900U
#define SERVO_CENTER_US 1500U
#define SERVO_LEFT_US 2200U
#define IR_NEAR_THRESHOLD 900U
#define IR_CLEAR_THRESHOLD 850U
#define SERVO_SETTLE_MS 100

static const uint16_t scan_pulses[] = {
    SERVO_RIGHT_US,
    SERVO_CENTER_US,
    SERVO_LEFT_US
};

static steering_state_t current_state = STEERING_LEG_1;
static uint8_t scan_index;
static uint16_t strongest_reading;
static uint16_t best_pulse;

static uint16_t read_at_pulse(uint16_t pulse_us) {
    servo_sendPulse(pulse_us);
    _delay_ms(SERVO_SETTLE_MS);
    return sensorsIR_read();
}

static uint16_t corner_turn_pulse(void) {
    switch (current_state) {
        case STEERING_CORNER_1:
        case STEERING_CORNER_3:
            return SERVO_RIGHT_US;
        case STEERING_CORNER_2:
            return SERVO_LEFT_US;
        case STEERING_LEG_1:
        case STEERING_LEG_2:
        case STEERING_LEG_3:
        case STEERING_LEG_4:
            return SERVO_CENTER_US;
    }

    return SERVO_CENTER_US;
}

static void begin_turn(void) {
    switch (current_state) {
        case STEERING_LEG_1:
            current_state = STEERING_CORNER_1;
            break;
        case STEERING_LEG_2:
            current_state = STEERING_CORNER_2;
            break;
        case STEERING_LEG_3:
            current_state = STEERING_CORNER_3;
            break;
        case STEERING_LEG_4:
        case STEERING_CORNER_1:
        case STEERING_CORNER_2:
        case STEERING_CORNER_3:
            return;
    }

    scan_index = 0;
    (void)read_at_pulse(corner_turn_pulse());
}

static void advance_to_next_leg(void) {
    switch (current_state) {
        case STEERING_CORNER_1:
            current_state = STEERING_LEG_2;
            break;
        case STEERING_CORNER_2:
            current_state = STEERING_LEG_3;
            break;
        case STEERING_CORNER_3:
            current_state = STEERING_LEG_4;
            break;
        case STEERING_LEG_1:
        case STEERING_LEG_2:
        case STEERING_LEG_3:
        case STEERING_LEG_4:
            return;
    }

    scan_index = 0;
    strongest_reading = 0;
    best_pulse = SERVO_CENTER_US;
    servo_sendPulse(SERVO_CENTER_US);
}

void steering_init(void) {
    current_state = STEERING_LEG_1;
    scan_index = 0;
    strongest_reading = 0;
    best_pulse = SERVO_CENTER_US;
    servo_sendPulse(SERVO_CENTER_US);
}

uint16_t steering_update(void) {
    if (current_state == STEERING_CORNER_1 ||
        current_state == STEERING_CORNER_2 ||
        current_state == STEERING_CORNER_3) {
        uint16_t reading = read_at_pulse(corner_turn_pulse());
        if (reading <= IR_CLEAR_THRESHOLD) {
            advance_to_next_leg();
        }
        return reading;
    }

    uint16_t pulse = scan_pulses[scan_index];
    uint16_t reading = read_at_pulse(pulse);

    if (reading >= IR_NEAR_THRESHOLD && current_state != STEERING_LEG_4) {
        begin_turn();
        return reading;
    }

    if (reading > strongest_reading) {
        strongest_reading = reading;
        best_pulse = pulse;
    }

    scan_index++;
    if (scan_index >= sizeof(scan_pulses) / sizeof(scan_pulses[0])) {
        servo_sendPulse(best_pulse);
        scan_index = 0;
        strongest_reading = 0;
        best_pulse = SERVO_CENTER_US;
    }

    return reading;
}

steering_state_t steering_get_state(void) {
    return current_state;
}
