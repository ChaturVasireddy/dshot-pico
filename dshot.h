#include "pico/stdlib.h"
#include "hardware/pio.h"
#include <stdint.h>

#pragma once

typedef struct {
    PIO pio;
    uint sm;
    uint offset;
    uint pin;
} dshot_actuator;

void dshot_init(dshot_actuator *actuator);
void dshot_arm(const dshot_actuator *actuator);
void mode3d(const dshot_actuator *actuator);
void thrust(const dshot_actuator *actuator, uint16_t throttle);