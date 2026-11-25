
#include "fan_thrust.h"
#include <util/delay.h>

int main(void)
{
	fan_init();
	while(1)
	{
	fan_on();
	// _delay_ms(10000);
	// fan_off();
	// _delay_ms(10000);

	}



}
