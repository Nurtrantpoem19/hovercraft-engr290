
#include "fan_thrust.h"
#include <avr/io.h>


void fan_init()
{
	DDRD |= (1 << PD4);
    PORTD &= ~(1 << PD4);
}


void fan_on()
{
    PORTD |= (1 << PD4); 


}
void fan_off()
{
    PORTD &= ~(1 << PD4); 
}
