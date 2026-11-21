
#ifndef ACCEL_H
#define ACCEL_H

#include <stdint.h>
#include "m3208.h"

typedef struct 
{
    
    uint8_t X_CHANNEL;    
    uint8_t Y_CHANNEL;
    uint8_t Z_CHANNEL;

    uint32_t x;    
    uint32_t y;    
    uint32_t z;

} Accel_struct;

Accel_struct init_Accellerometer ( uint8_t xyz_chanels[3] );

void read_Accel( Accel_struct* Accel );

#endif