#include "adc.h"
#include <avr/io.h>
// in this section of code
// we will initialize PC0 to be an ADC pin
// switch the adc channel to 0
// start conversion
// read bit
//

uint16_t sensorsIR_raw_to_mv(uint16_t raw_adc);
uint16_t sensorsIR_read();

void sensorsUS_init();
uint16_t sensorsUS_read();
