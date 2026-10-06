
#include "joystick.h"

#define X_CHANNEL 0
#define Y_CHANNEL 1

#define SPI_PATH    "/dev/spidev0.0"
#define GPIO_PATH   "/dev/gpiochip2"
#define GPIO_OFFSET 11
#define SPI_SPEED   10000
#define SPI_BPT     8 // bits per transfer

#define INTRODUCTION_MSG "-------- Welcome to Reaction Timer ----------- \n"


#define RULES_1 " Rules : You will receive a getReady message , after a few moments a LED will Turn on. \n"
#define RULES_2 "         Follwoing the led a direction will be displayed on screen.\n"
#define RULES_3 "         Press the joystick in that direction as fast as possible.\n"
#define RULES_4 "                       Press down the joystick to begin                \n"

static JoyStick joystick = {0};

void print_introduction(void);
void gameloop( void );

int main()
{

    // Init Joystick objects
    joystick = JoyStick_Init(  X_CHANNEL , Y_CHANNEL,
                                        SPI_PATH , SPI_SPEED ,
                                        GPIO_PATH , GPIO_OFFSET,
                                        SPI_BPT 
                                    );

    print_introduction();
    gameloop();

    return 0;
}

void gameloop( )
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
    while(1)
    {
        JoyStick_Read(&joystick);
        printf( "X_pos : %f , Y_pos : %f , Sel : %d\n" , joystick.x_pos , joystick.y_pos , joystick.sel);
    }
}

void print_introduction( void )
{
    printf( RULES_1);
    printf( RULES_2);
    printf( RULES_3);
    printf( RULES_4);

    while( joystick.sel != 1 )
    {
        JoyStick_Read(&joystick);
    }
 
}

void check_start_position()
{
    JoyStick_Read(&joystick);

    if( joystick.x_pos > DEADZONE || joystick.y_pos > DEADZONE)
    {
        printf("Return joystick to center to begin");

        while( joystick.x_pos > DEADZONE || joystick.y_pos > DEADZONE)
        {
            JoyStick_Read(&joystick);
        }
    }
}