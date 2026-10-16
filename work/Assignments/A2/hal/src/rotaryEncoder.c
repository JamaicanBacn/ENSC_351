


#include "rotaryEncoder.h"

Rotary_encoder init_Rotary_encoder( char* gpio_path1 , int* offsets )
{
    if( gpio_init( gpio_path1 , offsets , 2) == -1 )
    {
        exit(1);
    }

    Rotary_encoder r = {
        .gpio_offsets = *offsets,
        .clk = 0,
        .DT  = 0,
        .sel = 1,
        .position = 0   
    };

    return r;

}

void read_rotary( Rotary_encoder* r )
{
    r->clk  = gpio_read( r->gpio_offsets[0]);
    r->DT   = gpio_read( r->gpio_offsets[1]);
}

