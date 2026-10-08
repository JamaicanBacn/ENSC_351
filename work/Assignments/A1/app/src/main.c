
#include "joystick.h"
#include "LED.h"
#include <time.h>
#include <math.h>

#define X_CHANNEL 0
#define Y_CHANNEL 1

#define SPI_PATH    "/dev/spidev0.0"
#define SPI_SPEED   10000
#define SPI_BPT     8 // bits per transfer

#define GPIO_PATH   "/dev/gpiochip2"
#define GPIO_OFFSET 11

#define LED_GREEN_BRIGHTNESS   "/sys/class/leds/ACT/brightness"
#define LED_RED_BRIGHTNESS     "/sys/class/leds/PWR/brightness"

#define LED_GREEN_TRIGGER   "/sys/class/leds/ACT/trigger"
#define LED_RED_TRIGGER     "/sys/class/leds/PWR/trigger"

#define INTRODUCTION_MSG "-------- Welcome to Reaction Timer ----------- \n"


#define RULES_1 " Rules : You will receive a getReady message , after a few moments a LED will Turn on. \n"
#define RULES_2 "         Follwoing the led a direction will be displayed on screen.\n"
#define RULES_3 "         Press the joystick in that direction as fast as possible.\n"
#define RULES_4 "                       Press down the joystick to begin                \n"

static JoyStick joystick = {0};
static FILE* LED_GREEN = NULL;
static FILE* LED_RED   = NULL; 

void print_introduction(void);
void gameloop( void );
void get_ready_led( void );

static long long getTimeInMs(void)
{
    struct timespec spec;
    clock_gettime( CLOCK_REALTIME, &spec);
    long long seconds = spec.tv_sec;
    long long nanoSeconds = spec.tv_nsec;
    long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;

    return milliSeconds;

}

static void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

int Up_or_down();

int main()
{

    // Init Joystick objects
    joystick = JoyStick_Init(  X_CHANNEL , Y_CHANNEL,
                                        SPI_PATH , SPI_SPEED ,
                                        GPIO_PATH , GPIO_OFFSET,
                                        SPI_BPT 
                                    );

    
    LED_GREEN = init_led( LED_GREEN_BRIGHTNESS , LED_GREEN_TRIGGER);
    LED_RED   = init_led( LED_RED_BRIGHTNESS , LED_RED_TRIGGER  );

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
    print_introduction();

    while(1)
    {
        JoyStick_Read(&joystick);
        printf( "X_pos : %f , Y_pos : %f , Sel : %d UpDown %d\n" , joystick.x_pos , joystick.y_pos , joystick.sel , Up_or_down());
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

int check_start_position()
{
    JoyStick_Read(&joystick);

    if( fabs(joystick.x_pos) > DEADZONE || fabs(joystick.y_pos) > DEADZONE)
    {
        printf("Return joystick to center to begin");

        while( joystick.x_pos > DEADZONE || joystick.y_pos > DEADZONE)
        {
            JoyStick_Read(&joystick);
        }
    }

    return 1;
}

int check_ready_position()
{
    JoyStick_Read(&joystick);
    if( fabs(joystick.x_pos) > DEADZONE || fabs(joystick.y_pos) > DEADZONE)
    {
        printf( "Too soon");
        return -1;
    }

    return 0;
}

void get_ready_led( void )
{
    for( int i = 0; i < 4 ; i++ ){
        
        write_to_led( LED_GREEN , "1");
        write_to_led( LED_RED ,   "0");
        sleepForMs(250);

        write_to_led( LED_GREEN , "0" );
        write_to_led( LED_RED , "1"   );

        sleepForMs(250);
    }

    write_to_led(LED_GREEN  , "0");
    write_to_led(LED_RED    , "0");
}

int begin_game()
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

    get_ready_led();
    unsigned long long Delay = ( rand() % 3000 ) + 500 ; 
    bool Up = Delay % 2 == 0;
    sleepForMs( Delay );

    if( check_ready_position() == -1 )
    {
        return -1; // moved the joystick too early
    }

    if( Up )
    {
        printf( "UP" );
        write_to_led(LED_GREEN , "1");
    }
    else
    {
        printf("DOWN");
        write_to_led(LED_RED , "1");
    }

    unsigned long long start_time = getTimeInMs();
    unsigned long long end_time = start_time;
    int input;

    while( !input && (start_time - end_time) < 5000 )
    {
        JoyStick_Read(&joystick);
        end_time = getTimeInMs();
        input = Up_or_down();
    
    }


    return end_time - start_time;
}

// 1 -> UP , -1 -> DOWN , 0 -> no input
int Up_or_down()
{
    float prev_y = joystick.y_pos; // so one time errors dont ruin

    JoyStick_Read(&joystick);

    if( joystick.y_pos > DEADZONE && prev_y > DEADZONE) return 1;
    if( joystick.y_pos < -DEADZONE && prev_y < -DEADZONE) return -1;

    return 0;
}