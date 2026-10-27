
#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "gpio.h"
#include <stdbool.h>


typedef struct 
{
    uint32_t gpio_offsets[2];
    GPIOCHIP* gp_chip;

    uint8_t clk : 1;
    uint8_t DT  : 1; 
    uint8_t sel : 1;

    uint8_t prev_state : 2;
    uint8_t start_state : 2;

    int position;
    int micro_pos;

} Rotary_encoder;

Rotary_encoder init_Rotary_encoder( GPIOCHIP* gp_chip  , uint32_t offsets[2] );

void read_rotary( Rotary_encoder* r );





#endif