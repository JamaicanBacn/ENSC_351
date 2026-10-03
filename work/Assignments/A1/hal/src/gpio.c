
#include "gpio.h"



int gpio_init( struct gpiod_line *sel_line )
{
    // Requests the line that GPIO18 writes to
    sel_line = gpiod_line_find("GPIO18");

    if( sel_line = NULL)
    {
        perror("could not find GPIO18");
        return -1;
    }

    /*
        gpio_line_request_input_flags
        ARGUMENTS
            - gpio line that will be requested
            - name of the line 
            - Other flags
    */

    int gpio_init = gpio_line_request_input_flags
        ( 
        sel_line ,
        "joystick_sel" ,
        GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP
        );

    if( gpio_init < 0 )
    {
        perror("Could not request GPIO18");
        return -1;
    }

}

int gpio_read( struct gpiod_line *sel_line )
{
    int gpio_read_value = gpio_line_get_value(sel_line);

    //gpio_read_value < 0 indicated it failed

    if( gpio_read_value < 0 )
    {
        perror("GPIO_READ FAILED");
        return -1;
    }

    // button is active low , return the inverse
    return !gpio_read_value;
}