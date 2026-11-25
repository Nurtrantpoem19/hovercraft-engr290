#define F_CPU 16000000UL

#include <util/delay.h>
#include "servo.h"

int main(void) {
	servo_init();

	while (1) {


		// servo_sendPulse(1500);
		// _delay_ms(2000);
		//
		// servo_sendPulse(2200);
		// _delay_ms(2000);

		servo_center();// Minimum test position
		_delay_ms(6000);

		servo_setAngle(160);
		_delay_ms(2000);
		servo_setAngle(10);
		_delay_ms(2000);
		
		





	}
}
