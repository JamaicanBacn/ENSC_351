
#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <stdint.h>
#include <gpiod.h>

int gpio_init( const char* chip_path , uint64_t* offsets , int offset_count);
int gpio_read( uint64_t offset );
int gpio_close( void );

#endif
