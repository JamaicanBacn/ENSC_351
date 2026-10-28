
#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#define ROTARY_CYCLE_LENGTH 4

#include "gpio.h"
#include <stdbool.h>

typedef enum 
{
    S0 = 3,
    S1 = 1,
    S2 = 0,
    S3 = 2
} states;

typedef struct 
{
    uint32_t gpio_offsets[2];
    GPIOCHIP* gp_chip;

    uint8_t clk : 2;
    uint8_t DT  : 2; 
    uint8_t sel : 1;

    uint8_t prev_state : 2;
    int position;
    int transitions;

} Rotary_encoder;

Rotary_encoder init_Rotary_encoder( GPIOCHIP* gp_chip  , uint32_t offsets[2] );
void* rotary_handler( void* r_enc);
void read_rotary( Rotary_encoder* r );





#endif