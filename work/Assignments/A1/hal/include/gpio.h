
#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <gpiod.h>

int gpio_init( const char* chip_path , unsigned int offset );
int gpio_read( void );
int gpio_close( void );

#endif
