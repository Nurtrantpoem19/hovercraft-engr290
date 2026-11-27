#define F_CPU 16000000UL

#include <util/delay.h>
#include "../include/adc.h"
#include "../include/fan_thrust.h"
#include "../include/servo.h"
#include "../include/steering.h"
#include "../include/uart.h"

static void print_state(steering_state_t state) {
    switch (state) {
        case STEERING_LEG_1:
            uart_transmit_string("LEG 1");
            break;
        case STEERING_LEG_2:
            uart_transmit_string("LEG 2");
            break;
        case STEERING_LEG_3:
            uart_transmit_string("LEG 3");
            break;
        case STEERING_LEG_4:
            uart_transmit_string("LEG 4");
            break;
        case STEERING_CORNER_1:
            uart_transmit_string("CORNER 1");
            break;
        case STEERING_CORNER_2:
            uart_transmit_string("CORNER 2");
            break;
        case STEERING_CORNER_3:
            uart_transmit_string("CORNER 3");
            break;
    }
}

int main(void) {
    uart_init(F_CPU, 9600);
    adc_init();
    servo_init();
    fan_init();
    steering_init();

    uart_transmit_string("Steering initialized. Secure the fan before operation.\r\n");
    fan_on();

    while (1) {
        uint16_t ir_reading = steering_update();
        uart_transmit_string("IR raw: ");
        uart_transmit_uint16(ir_reading);
        uart_transmit_string(" | state: ");
        print_state(steering_get_state());
        uart_transmit_string("\r\n");
        _delay_ms(20);
    }
}
