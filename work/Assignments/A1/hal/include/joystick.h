
#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>
#include <stdbool.h>
#include "m3208.h"

typedef struct {
    uint8_t x_channel; // channel for X ADC
    uint8_t y_channel; // channel for Y ADC
    float x_pos;
    float y_pos;
    bool sel;

} JoyStick;

void JoyStick_Read( JoyStick* joystick );

#endif