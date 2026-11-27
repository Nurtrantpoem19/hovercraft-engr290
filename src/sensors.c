#include "../include/sensors.h"
#include <util/delay.h>
#include <stdint.h>


#define IR_ADC_MAX_RAW 1023U

static float sensorsIR_raw_to_cm(uint16_t raw);

uint16_t sensorsIR_read()
{
    admux_set_channel(ADC_CHANNEL_0);
    adc_start_conversion();
    while(adc_is_converting());
    (void)adc_read_10bit();

    adc_start_conversion();
    while(adc_is_converting());
    return adc_read_10bit();
}

uint16_t sensorsIR_raw_to_mv(uint16_t raw_adc) {
    return (uint16_t)(((uint32_t)raw_adc * 5000UL) / 1023UL);
}

uint16_t sensorsIR_average()
{
    float total_cm = 0.0f;
    for (uint8_t i = 0; i < 5; i++) {
        total_cm += sensorsIR_raw_to_cm(sensorsIR_read());
        _delay_ms(50);
    }

    return (uint16_t)(total_cm / 5.0f);
}


static float sensorsIR_raw_to_cm(uint16_t raw) {
    if (raw > IR_ADC_MAX_RAW) {
        raw = IR_ADC_MAX_RAW;
    }

    float x = (float)raw;

    /* Horner's method: ((c3 * x + c2) * x + c1) * x + c0 */
    const float c3 = -5.4068474e-6f;
    const float c2 =  4.4217541e-3f;
    const float c1 = -1.3056177f;
    const float c0 =  189.57182f;

    float cm = (((c3 * x + c2) * x + c1) * x + c0);
    return (cm < 0.0f) ? 0.0f : cm;
}


#define TRIG_PIN PB3
#define ECHO_PIN PD2


#define TRIG2_PIN PB5
#define ECHO2_PIN PD3



void sensorsUS_init()
{
    DDRB |= ((1 << TRIG_PIN) | (1 << TRIG2_PIN));
	DDRD &= ~(1 << ECHO_PIN);
	DDRD &= ~(1 << ECHO2_PIN);
    
    PORTB &= ~((1 << TRIG_PIN) | (1 << TRIG2_PIN));
    
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

uint16_t sensorsUS2_read(void)
{
    PORTB |= (1 << TRIG2_PIN);
    _delay_us(10);
    PORTB &= ~(1 << TRIG2_PIN);

    uint16_t timeout = 10000;
    while (!(PIND & (1 << ECHO2_PIN)))
    {
        if (--timeout == 0) return 122;
    }

    TCNT1 = 0;

    while (PIND & (1 << ECHO2_PIN))
    {
        if (TCNT1 >= 60000) break;
    }

    uint16_t ticks = TCNT1;
    return ticks / 116;
}
