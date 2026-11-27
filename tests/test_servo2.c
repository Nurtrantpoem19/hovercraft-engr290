#define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <util/delay.h>
#include "servo.h"
#include "servo2.h"

#define START_PULSE_US 2420U
#define MIN_PULSE_US 670U
#define PULSE_STEP_US 50U

int main(void) {
    servo_init();
    servo2_init();
    servo_center();
    sei();




    while (1) {
        servo2_center();
        _delay_ms(1000);
        servo2_sendPulse(MIN_PULSE_US);
        _delay_ms(1000);
        servo2_sendPulse(START_PULSE_US);
        _delay_ms(1000);

    }
}
