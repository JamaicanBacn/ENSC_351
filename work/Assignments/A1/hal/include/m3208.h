
#ifndef M_3208_H
#define M_3208_H

#include "spi.h"

#define M3208_TRANSMISSION_INIT 0x6 // 0000 0110 SIG/DIFF , START_BIT , 5 Padding 0's
#define BPT  3 // Bytes per transfer

int m_3208_read( uint8_t channel );

#endif