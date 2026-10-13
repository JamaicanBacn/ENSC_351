

#include "LED_RINGS.h"

#include <stdio.h>
#include <stdlib.h>

#define USB0_path "/dev/ttyUSB0"



int main()
{

    LED_STRUCT test_struct = LED_init( USB0_path );

    if( test_struct.usb_fd == -1)
    {
        perror("LED_INIT FAILED");
        exit(1);
    }

    for( int i = 0; i < 100 ; i++ )
    {

        LED_set_colour( test_struct , 0 , RED);
        usleep(50000);
        LED_set_colour( test_struct , 0 , GREEN);
        usleep(50000);
        LED_set_colour( test_struct , 0 , BLUE);
        usleep(50000);

    }

    sleep(2);


    return 0;
}