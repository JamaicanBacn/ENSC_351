
#ifndef M_3208_H
#define M_3208_H

#include "spi.h"


typedef enum
{
   M3208_CHANNEL_0,        
   M3208_CHANNEL_1,
   M3208_CHANNEL_2,
   M3208_CHANNEL_3,
   M3208_CHANNEL_4,
   M3208_CHANNEL_5,
   M3208_CHANNEL_6,
   M3208_CHANNEL_7
} Channels;

#define M3208_TRANSMISSION_INIT 0x6 // 0000 0110 SIG/DIFF , START_BIT , 5 Padding 0's
#define BPT  3 // Bytes per transfer

uint16_t m_3208_read( uint8_t channel );

#endif