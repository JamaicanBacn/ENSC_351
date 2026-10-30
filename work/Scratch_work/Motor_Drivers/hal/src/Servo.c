
#include "Servo.h"




int servo_init( char* i2c_path);
void set_servo_angle( uint8_t servo  , /*degrees*/ uint8_t angle )
{

    uint8_t buffer[2] = {
        servo,
        angle,
    };

    if( i2c_write( SERVO_I2C_ADDR , &buffer , 2 ) < 0)
    {
        exit(1)
    }

}   
