#define GPIO_PATH   "/dev/gpiochip2"
#define GPIO_OFFSET 11


#include "IR_receiver.h"


int main()
{

    init_IR_REC( GPIO_PATH , GPIO_OFFSET);
    int value;

    while( 1 )
    {
        value = read_IR_REC();
        printf("%d\n"  , value);

    }

    gpio_close();

    return 0;
}