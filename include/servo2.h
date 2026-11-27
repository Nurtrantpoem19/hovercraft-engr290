

#ifndef SERVO2_H
#define SERVO2_H

#include <stdint.h>

void servo2_init(void);
void servo2_sendPulse(uint16_t us);
uint8_t servo2_setAngle(int16_t angle);
void servo2_center(void);

#endif
