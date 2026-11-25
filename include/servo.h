#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

void servo_init(void);
void servo_sendPulse(uint16_t pulse_us);

// Sets an angle from 0 to 180; returns nonzero without moving for invalid
// input.
uint8_t servo_setAngle(int16_t angle);
void servo_center();
#endif
