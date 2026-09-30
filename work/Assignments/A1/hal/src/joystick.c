
#include "joystick.h"

void JoyStick_Read( JoyStick* joystick )
{
    uint16_t raw_x_value = m_3208_read( joystick->x_channel );
    uint16_t raw_y_value = m_3208_read( joystick->y_channel );

    joystick->x_pos = (raw_x_value - 2048) / 4095 ;
    joystick->y_pos = (raw_y_value - 2048) / 4095 ;

}