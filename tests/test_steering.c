#define F_CPU 16000000UL

#include <util/delay.h>
#include "adc.h"
#include "fan_thrust.h"
#include "servo.h"
#include "steering.h"

int main(void) {
    adc_init();
    servo_init();
    fan_init();
    steering_init();

    fan_on();

    while (1) {
        (void)steering_update();
        _delay_ms(20);
    }
}
