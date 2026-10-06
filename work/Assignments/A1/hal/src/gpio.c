
#include "gpio.h"

static struct gpiod_chip* chip = NULL; // what gpio chip is being used
static struct gpiod_line_request* request = NULL; // line request
static unsigned int gpio_offset; // line offset in the gpio file

int gpio_init( const char* chip_path , unsigned int offset)
{
    struct gpiod_line_settings*     settings = NULL;
    struct gpiod_request_config *   request_config = NULL;
    struct gpiod_line_config *      line_config = NULL;

    gpio_offset = offset;

    chip = gpiod_chip_open(chip_path); // check if the gpio acess 

    if( !chip )
    {
        perror("GPIOD CHIP OPEN FAILURE");
        return -1;
    }

    // set the line settings
    settings = gpiod_line_settings_new();

    if( !settings )
    {
        perror("GPIOD LINE SETTING FAILURE");
        return -1;
    }

    // set the direction of the pin
    if( gpiod_line_settings_set_direction(settings , GPIOD_LINE_DIRECTION_INPUT) < 0 )
    {
        perror("GPIO SET DIRECTION FAILURE");
        return -1;
    }

    line_config = gpiod_line_config_new();

    if( !line_config )
    {
        perror("GPIO_LINE_CONFIG_NEW FAILURE");
        return -1;
    }    

    if( gpiod_line_config_add_line_settings(
        line_config,
        &gpio_offset,
        1,
        settings) < 0 )
        {
            perror(" GPIOD LINE CONFIG ADD LINE SETTINGS FAILURE");
        }

    request = gpiod_chip_request_lines(
        chip,
        request_config,
        line_config
    );

    if( !request )
    {
        perror("GPIOD LINE REQUEST CONFIG");
        return -1;
    }

    gpiod_request_config_free(request_config);
    gpiod_line_config_free(line_config);
    gpiod_line_settings_free(settings);

    return 0;

}

int gpio_read( void )
{
    enum gpiod_line_value value;

    value = gpiod_line_request_get_value( request , gpio_offset);
    

    if( value == GPIOD_LINE_VALUE_ERROR )
    {
        perror("GPIO LINE READ FAILURE");
        return -1;
    }

    return value;

}