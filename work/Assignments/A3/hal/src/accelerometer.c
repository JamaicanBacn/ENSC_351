
#include "accellerometer.h"


Accel_struct init_Accellerometer( uint8_t xyz_channels[3] )
{
    Accel_struct acc = {
        .X_CHANNEL = xyz_channels[0],
        .Y_CHANNEL = xyz_channels[1],
        .Z_CHANNEL = xyz_channels[2],
    };

    return acc;
}

void read_Accel( Accel_struct* Accel )
{
    Accel->x = m_3208_read( Accel->X_CHANNEL );
    Accel->y = m_3208_read( Accel->Y_CHANNEL );
    Accel->z = m_3208_read( Accel->Z_CHANNEL );

}