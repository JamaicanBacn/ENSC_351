


#include "rotaryEncoder.h"

Rotary_encoder init_Rotary_encoder( GPIOCHIP* gp_chip, int offsets[2] )
{
    Rotary_encoder r_enc = {
        .gp_chip = gp_chip,
        .gpio_offsets = offsets,
        .sel = 0
    };

    // get the starting position , set postion to 0 to not count init
    read_rotary( &r_enc );
    r_enc.position = 0;

    return r_enc;
    
}

void read_rotary( Rotary_encoder* r )
{
    r->clk = gpio_read( r->gpio_offsets[0] , r->gp_chip );
    r->DT  = gpio_read( r->gpio_offsets[1] , r->gp_chip );

    uint8_t current_state   = r->clk | ( r->DT >> 1 );
    uint8_t prev_state      = r->prev_state;

    
    if( current_state == prev_state -1)
    {
        r->position--;
    }
    else if( current_state == prev_state + 1)
    {
        r->position++;
    }

    // anything other than this is a invalid state

    r->prev_state = current_state;

}

