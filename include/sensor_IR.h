#ifndef SENSOR_IR_H
#define SENSOR_IR_H
#include <stdint.h>

void adc_init(void);
uint16_t adc_read(void);

#endif