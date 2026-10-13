#include "../include/sensors.h"
#include <util/delay.h>

uint16_t sensorsIR_read()
{
    admux_set_channel(ADC_CHANNEL_0);
    adc_start_conversion();
    while(adc_is_converting());
    (void)adc_read_10bit;

    adc_start_conversion();
    while(adc_is_converting());
    return adc_read_10bit();
}

uint16_t sensorsIR_raw_to_mv(uint16_t raw_adc) {
    return (uint16_t)(((uint32_t)raw_adc * 5000UL) / 1023UL);
}


#define TRIG_PIN PB3
#define ECHO_PIN PD2
void sensorsUS_init()
{
    DDRB |= (1 << TRIG_PIN);
	DDRD &= ~(1 << ECHO_PIN);
    
    PORTB &= ~(1 << TRIG_PIN);
    
    TCCR1A = 0x00;
    TCCR1B = (1 << CS11); 

}

uint16_t sensorsUS_read()
{
    PORTB |= (1 << TRIG_PIN);
    _delay_us(10);
    PORTB &= ~(1 << TRIG_PIN);

    uint16_t timeout = 10000;
    while(!(PIND & (1 << ECHO_PIN)))
    {
        if(--timeout == 0) return 122;

    }


    TCNT1 = 0;

    while(PIND & (1 << ECHO_PIN))
    {
        if(TCNT1 >= 60000) break;
    }
    uint16_t ticks = TCNT1;
    uint16_t distance = ticks/116;
    return distance;
}

