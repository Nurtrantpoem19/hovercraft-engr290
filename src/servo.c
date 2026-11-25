#include "servo.h"
#include <avr/io.h>



#define SERVO_MIN_PULSE_US  900U
#define CENTER 1500
#define SERVO_MAX_PULSE_US 2200U

void servo_init(void) {
	DDRB |= (1 << PB1);

	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);

	ICR1 = 39999;

	OCR1A = 3000;
}

void servo_sendPulse(uint16_t us) {
    OCR1A = us * 2;
}

uint8_t servo_setAngle(int16_t angle) {
    if (angle < 0 || angle > 180) {
        return 1;
    }

    uint16_t pulse_range = SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US;
    uint16_t whole_step = pulse_range / 180U;
    uint16_t remainder_step = pulse_range % 180U;
    uint16_t pulse_offset = (uint16_t)(angle * whole_step)
        + (uint16_t)(angle * remainder_step) / 180U;
    uint16_t pulse_us = SERVO_MIN_PULSE_US + pulse_offset;

    servo_sendPulse(pulse_us);
    return 0;
}

void servo_center()
{
    servo_sendPulse(CENTER);

}

