#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

void servo_init(void);
void servo_sendPulse(uint16_t pulse_us);

uint8_t servo_setAngle(int16_t angle);
#endif
