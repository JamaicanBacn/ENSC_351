


#include "rotaryEncoder.h"

Rotary_encoder init_Rotary_encoder( GPIOCHIP* gp_chip, int offsets[2] )
{
    Rotary_encoder r_enc = {
        .clk    = 0,
        .DT     = 0,
        .gpio_offsets = offsets,
        .position = 0,
        .sel = 0
    };
}

void read_rotary( Rotary_encoder* r )
{
   return;
}

