#ifndef GYRON_CONTROLLER_H
#define GYRON_CONTROLLER_H

#include <stdint.h>

uint8_t gyro_controller_init(void);
uint8_t gyro_controller_update(float elapsed_seconds);

#endif