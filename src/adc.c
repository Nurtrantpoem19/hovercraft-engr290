
/*
 * This module provides functions for controlling the ATmega328P ADC through
 * the ADCSRA register. It supports enabling and disabling the ADC, starting
 * and monitoring conversions, configuring auto-triggering, checking and
 * clearing the ADC interrupt flag, enabling and disabling ADC interrupts, and
 * selecting the ADC prescaler while preserving unrelated register bits.
 */

#include "../include/adc.h"
#include <avr/io.h>
#include <stdbool.h>


void adc_init()
{
   admux_set_reference(ADC_REF_AREF);
   admux_set_channel(ADC_CHANNEL_0);
   adc_set_prescaler(ADC_PRESCALER_128);


   DIDR0 |= (1 << ADC0D);

   adc_enable();
}



void adc_enable(void) {
    ADCSRA |= (1 << ADEN);
}

void adc_disable(void) {
    ADCSRA &= ~(1 << ADEN);
}

void adc_start_conversion(void) {
    ADCSRA |= (1 << ADSC);
}

bool adc_is_converting(void) {
    return (ADCSRA & (1 << ADSC)) != 0;
}

void adc_enable_auto_trigger(void) {
    ADCSRA |= (1 << ADATE);
}

void adc_disable_auto_trigger(void) {
    ADCSRA &= ~(1 << ADATE);
}

bool adc_has_interrupt_flag_set(void) {
    return (ADCSRA & (1 << ADIF)) != 0;
}

void adc_clear_interrupt_flag(void) {
    ADCSRA |= (1 << ADIF);
}

void adc_enable_interrupt(void) {
    ADCSRA |= (1 << ADIE);
}

void adc_disable_interrupt(void) {
    ADCSRA &= ~(1 << ADIE);
}

void adc_set_prescaler(adc_prescaler_t prescaler) {
    ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
    ADCSRA |= (prescaler & 0x07);
}


//-------------------------------------------
void admux_set_reference(adc_ref_t ref) {
    ADMUX &= ~((1 << REFS1) | (1 << REFS0));
    ADMUX |= ((ref & 0x03) << REFS0);
}

void admux_set_left_adjust(bool enable) {
    if (enable) {
        ADMUX |= (1 << ADLAR);
    } else {
        ADMUX &= ~(1 << ADLAR);
    }
}

void admux_set_channel(adc_channel_t channel) {
    ADMUX &= 0xF0;
    ADMUX |= (channel & 0x0F);
}

//above is all admux configurations

//--------------------------------------


//these next three apis are reading a seperate register than previously

uint16_t adc_read_10bit(void) {
    return ADC;
}


