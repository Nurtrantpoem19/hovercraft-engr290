#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>
void servo_init();

void servo_sendPulse(uint16_t);
#endif
