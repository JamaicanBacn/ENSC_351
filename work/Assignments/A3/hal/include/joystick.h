
#ifndef JOYSTICK_H
#define JOYSTICK_H

#define DEADZONE 0.2f // 20 percent deadzone 

#include <stdint.h>
#include <stdbool.h>
#include "m3208.h"
#include "gpio.h"

typedef struct {

    uint8_t x_channel; // channel for X ADC
    uint8_t y_channel; // channel for Y ADC
    GPIOCHIP* gp_chip;
    uint32_t gpio_offset;
    float x_pos;
    float y_pos;
    bool sel;

} JoyStick;

JoyStick JoyStick_Init( uint8_t xy_channels[2],
                        GPIOCHIP* gp_chip,
                        uint32_t sel_offset
                        );

void JoyStick_Read( JoyStick* joystick );

#endif