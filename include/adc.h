
/*
 * This header defines the ADC prescaler options and declares functions for
 * enabling and disabling the ADC, starting and monitoring conversions,
 * configuring auto-triggering, managing the ADC interrupt flag, enabling and
 * disabling ADC interrupts, and selecting the ADC clock prescaler.
 */

#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include <stdbool.h>

void adc_init();

typedef enum
{
    ADC_PRESCALER_2_ALT = 0,
    ADC_PRESCALER_2 = 1,
    ADC_PRESCALER_4 = 2,
    ADC_PRESCALER_8 = 3,
    ADC_PRESCALER_16 = 4,
    ADC_PRESCALER_32 = 5,
    ADC_PRESCALER_64 = 6,
    ADC_PRESCALER_128 = 7
} adc_prescaler_t;

//---------------------
typedef enum
{
    ADC_REF_AREF = 0,
    ADC_REF_AVCC = 1,
    ADC_REF_RESERVED = 2,
    ADC_REF_INTERNAL = 3
} adc_ref_t;

typedef enum
{
    ADC_CHANNEL_0 = 0,
    ADC_CHANNEL_1 = 1,
    ADC_CHANNEL_2 = 2,
    ADC_CHANNEL_3 = 3,
    ADC_CHANNEL_4 = 4,
    ADC_CHANNEL_5 = 5,
    ADC_CHANNEL_6 = 6,
    ADC_CHANNEL_7 = 7,

} adc_channel_t;

void admux_set_reference(adc_ref_t ref);
void admux_set_left_adjust(bool enable);
void admux_set_channel(adc_channel_t channel);

// above is all admux register configurations
//--------------------------

void adc_enable(void);
void adc_disable(void);

void adc_start_conversion(void);
bool adc_is_converting(void);

void adc_enable_auto_trigger(void);
void adc_disable_auto_trigger(void);

bool adc_has_interrupt_flag_set(void);
void adc_clear_interrupt_flag(void);

void adc_enable_interrupt(void);
void adc_disable_interrupt(void);

void adc_set_prescaler(adc_prescaler_t prescaler);

// these next apis use the ADCL and ADCH registers, and are seperate from the
// previous apis
uint16_t adc_read_10bit(void);

// uint8_t adc_read_8bit(void);
//
// uint16_t adc_read_blocking(void);
#endif
