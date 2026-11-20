
#include "joystick.h"

JoyStick JoyStick_Init( uint8_t xy_channel[2],
                        GPIOCHIP* gp_chip,
                        uint32_t sel_offset
                        )
{
    JoyStick joystick = {
        .x_channel = xy_channel[0],
        .y_channel = xy_channel[1],
        .gp_chip = gp_chip,
        .gpio_offset = sel_offset,
        .y_pos = 0,
        .x_pos = 0
    }; 

    return joystick;

}


void JoyStick_Read( JoyStick* joystick )
{
    uint16_t raw_x_value = m_3208_read( joystick->x_channel );
    uint16_t raw_y_value = m_3208_read( joystick->y_channel );

    // Normalized and Centered to [-1 , 1 ]
    joystick->x_pos = (float)(raw_x_value - 2048) / 2047 ;
    joystick->y_pos = (float)(raw_y_value - 2048) / 2047 ;

    joystick->sel = !gpio_read( joystick->gpio_offset , joystick->gp_chip);

}