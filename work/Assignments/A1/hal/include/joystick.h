
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

} JoyStick;

JoyStick JoyStick_Init( uint8_t x_channel,
                        uint8_t y_channel,
                        char* spi_path,
                        uint32_t speed,
                        uint32_t bits_per_transfer
                        );

void JoyStick_Read( JoyStick* joystick );

#endif