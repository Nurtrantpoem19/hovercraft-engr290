

#include "servo2.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define SERVO2_MIN_PULSE_US  670U
#define SERVO2_CENTER        1500U
#define SERVO2_MAX_PULSE_US  2420U

void servo2_init(void) {
    // Configure PD6 as output (P2)
    DDRD |= (1 << PD6);

    // Initial pulse width: 1500 us
    OCR1B = SERVO2_CENTER * 2;

    // Enable Timer1 overflow and compare B interrupts
    TIMSK1 |= (1 << TOIE1) | (1 << OCIE1B);
}

// Start each 20 ms period
ISR(TIMER1_OVF_vect) {
    PORTD |= (1 << PD6);
}

// End the pulse when OCR1B is reached
ISR(TIMER1_COMPB_vect) {
    PORTD &= ~(1 << PD6);
}

void servo2_sendPulse(uint16_t us) {
    OCR1B = us * 2;
}

uint8_t servo2_setAngle(int16_t angle) {
    if (angle < 0 || angle > 180) {
        return 1;
    }

    uint16_t pulse_range = SERVO2_MAX_PULSE_US - SERVO2_MIN_PULSE_US;
    uint16_t whole_step = pulse_range / 180U;
    uint16_t remainder_step = pulse_range % 180U;

    uint16_t pulse_offset = (uint16_t)(angle * whole_step)
        + (uint16_t)(angle * remainder_step) / 180U;

    uint16_t pulse_us = SERVO2_MIN_PULSE_US + pulse_offset;

    servo2_sendPulse(pulse_us);

    return 0;
}

void servo2_center(void) {
    servo2_sendPulse(SERVO2_CENTER);
}
