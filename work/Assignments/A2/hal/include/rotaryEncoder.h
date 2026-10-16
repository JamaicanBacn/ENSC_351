
#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "gpio.h"
#include <stdbool.h>

typedef struct 
{
    int gpio_offsets[2];

    bool clk;
    bool DT;
    bool sel;

    int position;

} Rotary_encoder;

Rotary_encoder init_Rotary_encoder( GPIOCHIP* gp_chip  , int offsets[2] );

void read_rotary( Rotary_encoder* r );





#endif