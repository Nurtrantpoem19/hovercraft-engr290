#ifndef STEERING_H
#define STEERING_H

#include <stdint.h>

typedef enum {
    STEERING_LEG_1,
    STEERING_LEG_2,
    STEERING_LEG_3,
    STEERING_LEG_4,
    STEERING_CORNER_1,
    STEERING_CORNER_2,
    STEERING_CORNER_3
} steering_state_t;

void steering_init(void);
uint16_t steering_update(void);
steering_state_t steering_get_state(void);

#endif
