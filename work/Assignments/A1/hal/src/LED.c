
#include "LED.h"

FILE* init_led(char * led_brightness , char* led_trigger)
{
    FILE* LED_trigger_path      = fopen(led_trigger , "w");
    FILE* LED_brightness_path   = fopen(led_brightness    , "w");

    if( LED_trigger_path == NULL || LED_brightness_path == NULL )
    {
        perror("LED OPENING FAILED");
        return NULL;
    }

    int written = fprintf( LED_trigger_path , "none");
    if( written <= 0 )
    {
        perror("TRIGGER FAILURE");
        return NULL;
    }

    fclose(LED_trigger_path);

    return LED_brightness_path;
}

int write_to_led( FILE* led_path , char* value )
{
    fseek( led_path , 0 , SEEK_SET);
    int charWritten = fprintf(led_path , "%s" , value);


    if( charWritten <= 0 )
    {
        perror("LED WRITE FAILED");
        return -1;
    }

    fflush(led_path);

    return 0;
}

int close_led( FILE* led_path)
{
    int closed = fclose( led_path );

    if( closed < 0)
    {
        perror("LED CLOSE FAILED");
        return -1;
    }

    return 0;
}