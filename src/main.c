#include <stdint.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "../include/uart.h"
#include "../include/adc.h"
#include "../include/sensors.h"
#include "../include/TWI_290.h"
#include "../include/imu.h"

static void print_fixed2(float value) {
    int32_t scaled = (int32_t)(value * 100.0f + (value < 0.0f ? -0.5f : 0.5f));
    uint32_t magnitude = (scaled < 0) ? (uint32_t)(-scaled) : (uint32_t)scaled;
    uint16_t whole = (uint16_t)(magnitude / 100);
    uint8_t digits = 1;
    uint16_t count = whole;

    while (count >= 10) {
        count /= 10;
        digits++;
    }

    uint8_t characters = digits + 3 + (scaled < 0);
    while (characters++ < 8) {
        uart_transmit_char(' ');
    }
    if (scaled < 0) {
        uart_transmit_char('-');
    }
    uart_transmit_uint16(whole);
    uart_transmit_char('.');
    uart_transmit_char((char)('0' + (magnitude / 10) % 10));
    uart_transmit_char((char)('0' + magnitude % 10));
}

static void print_imu_header(void) {
    uart_transmit_string("+----------+----------+----------+----------+----------+----------+\r\n");
    uart_transmit_string("| AX (g)   | AY (g)   | AZ (g)   | GX (d/s) | GY (d/s) | GZ (d/s) |\r\n");
    uart_transmit_string("+----------+----------+----------+----------+----------+----------+\r\n");
}

int main(void) {
    uart_init(F_CPU, 9600);
    uart_transmit_string("System Initialized. Starting sensors...\r\n");
    adc_init();
    sensorsUS_init();
    TWI_init();

    uint8_t imu_status = imu_init();
    if (imu_status != 0) {
        uart_transmit_string("IMU initialization failed (code ");
        uart_transmit_uint16(imu_status);
        uart_transmit_string(").\r\n");
        while (1) {
            _delay_ms(1000);
        }
    }

    uart_transmit_string("IMU ready; values are accel (g) and gyro (deg/s).\r\n");
    print_imu_header();
    uint8_t ir_counter = 0;

    while (1) {
        if (imu_read() != 0) {
            uart_transmit_string("IMU read failed.\r\n");
        } else {
            convert_accel_raw();
            convert_gyro_raw();

            uart_transmit_string("| ");
            print_fixed2(accel.x);
            uart_transmit_string(" | ");
            print_fixed2(accel.y);
            uart_transmit_string(" | ");
            print_fixed2(accel.z);
            uart_transmit_string(" | ");
            print_fixed2(gyro.x);
            uart_transmit_string(" | ");
            print_fixed2(gyro.y);
            uart_transmit_string(" | ");
            print_fixed2(gyro.z);
            uart_transmit_string(" |\r\n");
        }

        transmit_IR();
        ir_counter++;
        if (ir_counter >= 10) {
            transmit_US();
            ir_counter = 0;
        }

        _delay_ms(200);
    }

    return 0;
}
