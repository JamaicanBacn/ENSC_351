
#include "joystick.h"

JoyStick JoyStick_Init( uint8_t x_channel,
                        uint8_t y_channel,
                        char* spi_path,
                        uint32_t speed,
                        uint32_t bits_per_word
                        )
{
    JoyStick joystick = {
        .x_channel = x_channel,
        .y_channel = y_channel,
        .y_pos = 0,
        .x_pos = 0
    };

    if(spi_init( spi_path , speed , bits_per_word) < 0)
    {
        exit(1);
    }

    if( gpio_init() < 0 )
    {
        exit(1);
    }

    return joystick;

}


void JoyStick_Read( JoyStick* joystick )
{
    uint16_t raw_x_value = m_3208_read( joystick->x_channel );
    uint16_t raw_y_value = m_3208_read( joystick->y_channel );

    printf( "%d , %d : " , raw_x_value , raw_y_value);

    // Normalized and Centered to [-1 , 1 ]
    joystick->x_pos = (float)(raw_x_value - 2048) / 2047 ;
    joystick->y_pos = (float)(raw_y_value - 2048) / 2047 ;

    joystick->sel = gpio_read( joystick->sel_line);

}