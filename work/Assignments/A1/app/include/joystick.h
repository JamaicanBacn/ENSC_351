
#ifndef JOYSTICK_H
#define JOYSTICK_H

#define DEADZONE 0.05f // 5 percent deadzone 

#include <stdint.h>
#include <stdbool.h>
#include "m3208.h"
#include "gpio.h"

typedef struct {

    uint8_t x_channel; // channel for X ADC
    uint8_t y_channel; // channel for Y ADC
    struct  gpiod_line *sel_line;
    float x_pos;
    float y_pos;
    bool sel;

} JoyStick;

JoyStick JoyStick_Init( uint8_t x_channel,
                        uint8_t y_channel,
                        char* spi_path,
                        uint32_t speed,
                        char* gpio_path,
                        unsigned int gpio_offset,
                        uint32_t bits_per_transfer
                        );

void JoyStick_Read( JoyStick* joystick );

#endif