#define F_CPU 16000000UL

#include <util/delay.h>
#include "fan_lift.h"
#include "fan_thrust.h"
#include "sensors.h"
#include "uart.h"

int main(void) {
    uart_init(F_CPU, 9600);
    sensorsUS_init();
    fan_init();
    fan_lift_init();

    uart_transmit_string("Ultrasonic test with lift and thrust fans enabled\r\n");
    fan_on();
    fan_lift_on();

    while (1) {
        transmit_US();
        _delay_ms(250);
    }
}
