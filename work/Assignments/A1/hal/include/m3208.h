
#ifndef M_3208_H
#define M_3208_H

#include "spi.h"

#define M3208_TRANSMISSION_INIT 0b00000110 // SIG/DIFF , START_BIT , 5 Padding 0's

int m_3208_read( uint8_t channel );

#endif