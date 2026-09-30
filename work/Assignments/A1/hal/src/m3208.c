#include "m3208.h"


static uint32_t len = 12;

/*

0000 0110

*/
int m_3208_read( uint8_t channel )
{
    uint8_t tx[3] = {0};
    uint8_t rx[3] = {0};

    /*

    TRANSMISSION FORMAT:
    
    5_PADDING_0's | START_BIT | SGL/DIFF | 3_CHANNEL_BITS
    Data transmission will begin on the next clock
    0000 0SSC CCND DDDD

    One cycle delay following channel 
    Null bit is sent
    Then data Transmission

    /\/\/\/\/----\/---\/\/\/\/\
    |-------|-----|----|-------|
      MODE   DELAY NULL  DATA
    */


    tx[0] = M3208_TRANSMISSION_INIT | ( (channel & 0b100) >> 2);
    tx[1] = (channel & 0b011 ) << 6;

    if (spi_transfer( &tx , &rx , 3) < 0)
    {
        stderr("SPI TRANSFER FAILED");
        exit(1);
    }

    /*
    RECEIVING FORMAT:

    ???? ???? ???0 BBBB BBBB BBBB
    |   r0  |-|   r1  |-|   r2  |

    */

    return ( (rx[1] & 0b1111) << 8 ) | rx[2] ;


}