#include "sensor_IR.h"
#include <avr/io.h>




// Remove busy-waiting
/*
ADATE
ADIF set automatically and cleared by hardware
ADIE
*/

void adc_init(void) {
    SREG |= (1 << SREG_I); // Enables global interrupts --> Move to bottom of system_init later


    DIDR0 |= (1 << ADC3D); // Disables the Digital Input from the pin
    ADMUX = (1 << MUX1) | (1 << MUX0); // selects channel ADC3, REFS[0:1] kept at 0 to use AREF
    // Enable ADC , ADC Interrupt Enable , Start 1st conversion, Auto Trigger Enable, ADPS = 111 --> clock prescaler = 128
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    // ADC Auto Trigger Source: Timer/Counter1 Compare Match B
    ADCSRB = (1 << ADTS2) | (0 << ADTS1) | (1 << ADTS0);
}

uint16_t adc_read(void) {
    ADCSRA |= (1 << ADSC); // start conversion
    while (ADCSRA & (1 << ADSC)); // wait for conversion to finish
    return ADC; // 10-bit result stored in 16-bit ADCH | ADCL
}

// timers
/*
    A register: OCR1AH | OCR1AL
    B register: OCR1BH | OCR1BL
    Automatically compared with TCNT1
    TCNT1 = TCNT1H | TCNT1L



*/
void adc_timer(void) {
    TCCR1B = (1 << CS11) | (1 << CS10); // Prescaler: 64
    OCR0B = 24999; // 100ms: (0.1*FCPU/64 = 25000 ticks)
}