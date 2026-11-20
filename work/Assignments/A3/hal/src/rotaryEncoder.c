


#include "rotaryEncoder.h"

Rotary_encoder init_Rotary_encoder( GPIOCHIP* gp_chip, uint32_t offsets[3] )
{
    Rotary_encoder r_enc = {
        .gp_chip = gp_chip,
        .gpio_offsets[0] = offsets[0],
        .gpio_offsets[1] = offsets[1],
        .gpio_offsets[2] = offsets[2],

        .sel = 0,
        .transitions = 0,
        .position = 0,

        .clk = 0,
        .DT = 0,
        .exit = 0
    };

    return r_enc;
    
}

void read_rotary(Rotary_encoder* r)
{

    r->clk = gpio_read(r->gpio_offsets[DT],  r->gp_chip);
    r->DT  = gpio_read(r->gpio_offsets[CLK], r->gp_chip);
    r->sel = gpio_read(r->gpio_offsets[SEL], r->gp_chip);

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
        r->position = r->position < 5 ? r->position + 1 : r->position;
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