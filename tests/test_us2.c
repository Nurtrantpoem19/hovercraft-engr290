#define F_CPU 16000000UL

#include <util/delay.h>
#include "sensors.h"
#include "uart.h"

int main(void) {
    uart_init(F_CPU, 9600);
    sensorsUS_init();
    uart_transmit_string("Ultrasonic sensor 1 and 2 test\r\n");

    while (1) {
        uart_transmit_string("US1 Dist: ");
        uart_transmit_uint16(sensorsUS_read());
        uart_transmit_string(" cm\r\n");

        _delay_ms(60);

        uart_transmit_string("US2 Dist: ");
        uart_transmit_uint16(sensorsUS2_read());
        uart_transmit_string(" cm\r\n");
        _delay_ms(1050);
    }
}
