#include "fan_lift.h"
#include <util/delay.h>

int main(void) {
    fan_lift_init();

    while (1) {
        fan_lift_on();
        _delay_ms(5000);
        fan_lift_off();
        _delay_ms(5000);
    }
}
