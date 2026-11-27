#define F_CPU 16000000UL

#include <util/delay.h>
#include "gyron_controller.h"

#define UPDATE_INTERVAL_SECONDS 0.06f

int main(void) {
    if (gyro_controller_init() != 0U) {
        while (1) {
            _delay_ms(1000);
        }
    }

    while (1) {
        if (gyro_controller_update(UPDATE_INTERVAL_SECONDS) != 0U) {
            _delay_ms(100);
        }
        _delay_ms(20);
    }
}
