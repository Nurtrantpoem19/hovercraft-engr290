#include "servo.h"
#include <avr/io.h>

void servo_init()
{
	DDRB |= (1 << PB1);

	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);

	ICR1 = 39999;

	OCR1A = 3000;

}

void servo_sendPulse(uint16_t us) {
    OCR1A = us * 2;
}

