#include <avr/io.h>
#include "stdint.h"


uint16_t adc_read(void) {
    ADCSRA |= (1 << ADSC); // start conversion
    while (ADCSRA & (1 << ADSC)); // wait for conversion to finish
    return ADC; // 10-bit result stored in 16-bit ADCH | ADCL
}