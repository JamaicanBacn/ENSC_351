

#include "LED_RINGS.h"
#include "IR_receiver.h"

#include <stdio.h>
#include <stdlib.h>

#define USB0_path "/dev/ttyUSB0"

#define GPIO_PATH   "/dev/gpiochip2"
#define GPIO_OFFSET 11



int main()
{
    init_IR_REC( GPIO_PATH , GPIO_OFFSET);
    LED_STRUCT test_struct = LED_init( USB0_path );

    if( test_struct.usb_fd == -1)
    {
        perror("LED_INIT FAILED");
        exit(1);
    }

    while( 1 )
    {
        usleep(50000);
        if( read_IR_REC() )
        {
            LED_set_colour(test_struct , 0 , RED);
        }
        else{
            LED_set_colour(test_struct , 0 , BLUE);
        }
    }

    /*
    for( int i = 0; i < 100 ; i++ )
    {

        LED_set_colour( test_struct , 0 , RED);
        usleep(50000);
        LED_set_colour( test_struct , 0 , GREEN);
        usleep(50000);
        LED_set_colour( test_struct , 0 , BLUE);
        usleep(50000);

    }
    */

    sleep(2);


    return 0;
}