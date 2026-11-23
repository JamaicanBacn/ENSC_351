


#include "rotaryEncoder.h"
#include <stdlib.h>

Rotary_encoder init_Rotary_encoder( GPIOCHIP* gp_chip, const uint32_t offsets[3] )
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

    int clk = gpio_read(offsets[CLK], gp_chip);
    int dt = gpio_read(offsets[DT], gp_chip);
    if (clk >= 0 && dt >= 0) r_enc.prev_state = (clk << 1) | dt;
    return r_enc;
    
}

void read_rotary(Rotary_encoder* r)
{

    int clk = gpio_read(r->gpio_offsets[CLK], r->gp_chip);
    int dt = gpio_read(r->gpio_offsets[DT], r->gp_chip);
    int sel = gpio_read(r->gpio_offsets[SEL], r->gp_chip);
    if (sel >= 0) r->sel = sel;
    if (clk < 0 || dt < 0) return;
    r->clk = clk;
    r->DT = dt;

    uint8_t current = (clk << 1) | dt;
    uint8_t previous = r->prev_state;
    if (current == previous) return;

    // Clockwise: CLK falls before DT from the idle high/high state.
    static const int delta[4][4] = {
        { 0, -1,  1,  0},
        { 1,  0,  0, -1},
        {-1,  0,  0,  1},
        { 0,  1, -1,  0}
    };
    if ((current ^ previous) == 3) r->transitions = 0;
    else r->transitions += delta[previous][current];
    if (abs(r->transitions) == ROTARY_CYCLE_LENGTH) {
        r->position += r->transitions / ROTARY_CYCLE_LENGTH;
        r->transitions = 0;
    }
    r->prev_state = current;
}
