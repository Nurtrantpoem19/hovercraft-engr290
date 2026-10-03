#include <avr/io.h>

void adc_init(void) {
    DIDR0 |= (1 << ADC3D); // Disables the Digital Input from the pin
    ADMUX = (1 << MUX1) | (1 << MUX0); // selects channel ADC3, REFS[0:1] kept at 0 to use AREF
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // enable ADC | ADPS = 111 --> clock prescaler = 128
    ADCSRA |= (1 << ADSC); // Initialialize the ADC by starting the first conversion
    while (ADCSRA & (1 << ADSC)); // finishes initial ADC setup
}