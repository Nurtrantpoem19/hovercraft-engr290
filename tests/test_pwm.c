#define F_CPU 16000000UL

#include <util/delay.h>
#include "servo.h"

int main(void) {
	servo_init();

	while (1) {
		servo_sendPulse(1000); // Minimum test position
		_delay_ms(5000);

		servo_sendPulse(1500); // Center position
		_delay_ms(4000);

		servo_sendPulse(2000); // Maximum test position
		_delay_ms(2000);
	}
}
