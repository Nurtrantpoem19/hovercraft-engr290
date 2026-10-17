#include <stdint.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "../include/uart.h"
#include "../include/adc.h"
#include "../include/sensors.h"
#include "../include/TWI_290.h"

int main(void) {
    // Initialize hardware peripherals
    uart_init(F_CPU, 9600);

    uart_transmit_string("System Initialized. Reading IR Sensor...\r\n");
    adc_init();
    sensorsUS_init();


    uint16_t ir_counter = 0;

    while (1) {
        transmit_IR();
        ir_counter++;

        if (ir_counter >= 10)
        {
            transmit_US();
            ir_counter = 0; 
        }

        _delay_ms(200);
    }

    return 0;
}
