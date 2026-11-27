#define F_CPU 16000000UL

#include <util/delay.h>
#include "adc.h"
#include "sensors.h"
#include "uart.h"

int main(void) {
    uart_init(F_CPU, 9600);
    adc_init();
    uart_transmit_string("IR sensor test\r\n");

    while (1) {
        transmit_IR();
        _delay_ms(250);
    }
}
