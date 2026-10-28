


#include "rotaryEncoder.h"

Rotary_encoder init_Rotary_encoder( GPIOCHIP* gp_chip, uint32_t offsets[2] )
{
    Rotary_encoder r_enc = {
        .gp_chip = gp_chip,
        .gpio_offsets[0] = offsets[0],
        .gpio_offsets[1] = offsets[1],
        .sel = 0
    };

    // get the starting position , set postion to 0 to not count init
    read_rotary( &r_enc );
    r_enc.position = 0;
    return r_enc;
    
}

void* rotary_handler( void* r_enc)
{

    while(1)
    {
        read_rotary( r_enc );
    }

}

void read_rotary(Rotary_encoder* r)
{

    r->clk = gpio_read(r->gpio_offsets[0], r->gp_chip);
    r->DT  = gpio_read(r->gpio_offsets[1], r->gp_chip);

    int clk = r->clk;
    int dt  = r->DT;

    if (clk < 0 || dt < 0) {
        return;
    }

    uint8_t current = ((uint8_t)clk << 1) | (uint8_t)dt;
    uint8_t previous = r->prev_state;

    switch ( current )
    {
    case S0 :
            if( previous == S3)
            {
                r->transitions++;
            }
            else if( previous == S1)
            {
                r->transitions--;
            }
            else if( previous == S2)
            {
                r->transitions = 0;
            }
            else
            {
                r->transitions = 0;
            }

        break;
    
    case S1 :
            if( previous == S0)
            {
                r->transitions++;
            }
            else if( previous == S2)
            {
                r->transitions--;
            }
            else if( previous == S3)
            {
                r->transitions = 0;
            }
        break;

    case S2 :
            if( previous == S1)
            {
                r->transitions++;
            }
            else if( previous == S3)
            {
                r->transitions--;
            }
            else if( previous == S0)
            {
                r->transitions = 0;
            }

        break;

    case S3 :
            if( previous == S2)
            {
                r->transitions++;
            }
            else if( previous == S0)
            {
                r->transitions--;
            }
            else if( previous == S1)
            {
                r->transitions = 0;
            }

        break;

    default:
        break;
    }

    if( r->transitions >= ROTARY_CYCLE_LENGTH)
    {
        r->position++;
        r->transitions = 0;
    }
    
    if( r->transitions <= -ROTARY_CYCLE_LENGTH)
    {
        // removes negative positions
        r->position = r->position > 0 ? r->position - 1 : 0;
        r->transitions = 0;
    }

    r->prev_state = current;
    
}