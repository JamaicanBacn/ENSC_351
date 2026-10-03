
#include "joystick.h"

#define X_CHANNEL 0
#define Y_CHANNEL 1

#define SPI_PATH    "/dev/spidev0.0"
#define SPI_SPEED   100
#define SPI_BPT     8 // bits per transfer

#define INTRODUCTION_MSG "-------- Welcome to Reaction Timer ----------- \n"


#define RULES_1 " Rules : You will receive a getReady message , after a few moments a LED will Turn on. \n"
#define RULES_2 "         Follwoing the led a direction will be displayed on screen.\n"
#define RULES_3 "         Press the joystick in that direction as fast as possible.\n"
#define RULES_4 "                       Press down the joystick to begin                \n"

int main()
{

    // Init Joystick objects
    JoyStick joystick = JoyStick_Init(  X_CHANNEL , Y_CHANNEL,
                                        SPI_PATH , SPI_SPEED , SPI_BPT );
    
    while(1)
    {
        JoyStick_Read(&joystick);
        printf( "X_pos : %f , Y_pos : %f , Sel : %d" , joystick.x_pos , joystick.y_pos , joystick.sel);
    }

    return 0;
}

void gameloop( JoyStick JoyStick )
{
    /*
        LOOP
            print getready message
            turn Green LED on for 250ms 
            turn Red LEd on for 250ms
            make sure the joystick centered
            
            wait between 0.5 to 3s
            check if the joystick is centered
            make a directional decision
            record there time if it was the fastest in the session
    
    */

    print_introduction();

}

void print_introduction( void )
{
    printf( RULES_1);
    printf( RULES_2);
    printf( RULES_3);
    printf( RULES_4);
}