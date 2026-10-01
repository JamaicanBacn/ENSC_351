
#include "joystick.h"

#define X_CHANNEL 0
#define Y_CHANNEL 1

#define SPI_PATH    "/dev/spidev0.0"
#define SPI_SPEED   50000
#define SPI_BPT     8 // bits per transfer

int main()
{

    JoyStick joystick = JoyStick_Init(  X_CHANNEL , Y_CHANNEL,
                                        SPI_PATH , SPI_SPEED , SPI_BPT );

    while(1)
    {
        JoyStick_Read(joystick);
        printf( "X_pos : %f   Y_pos : %f \n" , joystick.x_pos , joystick.y_pos);
    }

    return 0;
}