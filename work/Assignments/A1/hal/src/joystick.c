
#include "joystick.h"

JoyStick JoyStick_Init( uint8_t x_channel,
                        uint8_t y_channel,
                        char* spi_path,
                        uint32_t speed,
                        uint32_t bits_per_transfer
                        )
{
    JoyStick joystick = {
        .x_channel = x_channel,
        .y_channel = y_channel,
        .y_pos = 0,
        .x_pos = 0;
    }

    if(spi_init( spi_path , speed , bits_per_transfer) < 0)
    {
        stderr("SPI_INIT_FAILURE");
        exit(1);
    }

}


void JoyStick_Read( JoyStick* joystick )
{
    uint16_t raw_x_value = m_3208_read( joystick->x_channel );
    uint16_t raw_y_value = m_3208_read( joystick->y_channel );

    joystick->x_pos = (raw_x_value - 2048) / 4095 ;
    joystick->y_pos = (raw_y_value - 2048) / 4095 ;

}