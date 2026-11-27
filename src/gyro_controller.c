#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "gyron_controller.h"
#include "TWI_290.h"
#include "imu.h"
#include "servo.h"
#include "uart.h"

static float normalize_yaw(float yaw) {
    while (yaw > 180.0f) {
        yaw -= 360.0f;
    }
    while (yaw < -180.0f) {
        yaw += 360.0f;
    }
    return yaw;
}

static void transmit_yaw(float yaw, uint8_t servo_angle) {
    int32_t yaw_hundredths = (int32_t)(yaw * 100.0f + (yaw < 0.0f ? -0.5f : 0.5f));
    uint32_t magnitude = (yaw_hundredths < 0)
        ? (uint32_t)(-yaw_hundredths)
        : (uint32_t)yaw_hundredths;

    uart_transmit_string("Yaw: ");
    if (yaw_hundredths < 0) {
        uart_transmit_char('-');
    }
    uart_transmit_uint16((uint16_t)(magnitude / 100U));
    uart_transmit_char('.');
    uart_transmit_char((char)('0' + (magnitude / 10U) % 10U));
    uart_transmit_char((char)('0' + magnitude % 10U));
    uart_transmit_string(" deg | Servo: ");
    uart_transmit_uint16(servo_angle);
    uart_transmit_string(" deg\r\n");
}

uint8_t gyro_controller_init(void) {
    uart_init(F_CPU, 9600);
    TWI_init();

    uint8_t status = imu_init();
    if (status != 0U) {
        uart_transmit_string("IMU init failed: ");
        uart_transmit_uint16(status);
        uart_transmit_string("\r\n");
        return status;
    }

    servo_init();
    servo_center();
    uart_transmit_string("Gyro controller ready\r\n");
    return 0;
}

uint8_t gyro_controller_update(float elapsed_seconds) {
    if (imu_read() != 0U) {
        uart_transmit_string("IMU read failed\r\n");
        return 1;
    }

    convert_accel_raw();
    convert_gyro_raw();
    rotation_update(elapsed_seconds);
    rotation.yaw = normalize_yaw(rotation.yaw);

    int16_t servo_angle = (int16_t)(90.0f + rotation.yaw);
    if (servo_angle < 0) {
        servo_angle = 0;
    } else if (servo_angle > 180) {
        servo_angle = 180;
    }

    uint8_t angle = (uint8_t)servo_angle;
    if (servo_setAngle(servo_angle) != 0U) {
        return 2;
    }

    transmit_yaw(rotation.yaw, angle);
    return 0;
}