
#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "gpio.h"
#include <stdbool.h>

typedef enum : uint8_t {
    S0,
    S1,
    S2,
    S3

} Rotary_states;

typedef struct 
{
    int gpio_offsets[2];
    GPIOCHIP* gp_chip;

    uint8_t clk : 1;
    uint8_t DT  : 1; 
    uint8_t sel : 1;

    uint8_t prev_state : 2;

    int position;

} Rotary_encoder;

Rotary_encoder init_Rotary_encoder( GPIOCHIP* gp_chip  , int offsets[2] );

void read_rotary( Rotary_encoder* r );





#endif