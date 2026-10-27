


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
    r_enc.micro_pos = 0;
    r_enc.start_state = r_enc.clk | r_enc.DT << 1;
    r_enc.prev_state = r_enc.start_state;

    return r_enc;
    
}

void read_rotary( Rotary_encoder* r )
{
 // Keep these as ints until errors have been checked.
    int clk = gpio_read(r->gpio_offsets[0], r->gp_chip);
    int dt  = gpio_read(r->gpio_offsets[1], r->gp_chip);

    if (clk < 0 || dt < 0) {
        return;
    }

    r->clk = clk;
    r->DT  = dt;

    uint8_t current_state = (clk << 1) | dt;

    // Four-bit value: [previous CLK, previous DT, CLK, DT].
    uint8_t transition = (r->prev_state << 2) | current_state;

    switch (transition) {
        // 00 -> 01 -> 11 -> 10 -> 00
        case 0x1:
        case 0x7:
        case 0xE:
        case 0x8:
            r->micro_pos++;
            break;

        // 00 -> 10 -> 11 -> 01 -> 00
        case 0x2:
        case 0xB:
        case 0xD:
        case 0x4:
            r->micro_pos--;
            break;

        default:
            // Unchanged state or invalid two-bit transition.
            break;
    }

    if( r->micro_pos > 3)
    {
        r->position++;
        r->micro_pos = 0;
    }
    else if( r->micro_pos < -3 )
    {
        r->position--;
        r->micro_pos = 0;
    }

    r->prev_state = current_state;
}

