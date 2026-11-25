#include "fan_lift.h"
#include <avr/io.h>

void fan_lift_init(void) {
    PORTD &= ~(1 << PD7);
    DDRD |= (1 << PD7);
}

void fan_lift_on(void) {
    PORTD |= (1 << PD7);
}

void fan_lift_off(void) {
    PORTD &= ~(1 << PD7);
}