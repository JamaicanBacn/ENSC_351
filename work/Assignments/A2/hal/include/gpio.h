
#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <stdint.h>
#include <gpiod.h>

typedef enum{
    GPIO_11_OFFSET,

} GPIO_CHIP1_OFFSETS;

// GPIOCHIP0
typedef enum {
    GPIO_2_OFFSET  = 18,
    GPIO_3_OFFSET  = 17,
    GPIO_7_OFFSET  = 9,
    GPIO_8_OFFSET  = 0,
    GPIO_9_OFFSET  = 4,
    GPIO_10_OFFSET = 3,
    GPIO_11_OFFSET = 2,
    GPIO_23_OFFSET = 7,
    GPIO_24_OFFSET = 10

} GPIOCHIP0_OFFSETS;


// GPIOCHIP1
typedef enum {
    GPIO_4_OFFSET  = 38,
    GPIO_22_OFFSET = 41,
    GPIO_25_OFFSET = 42,
    GPIO_26_OFFSET = 36,
    GPIO_27_OFFSET = 33

} GPIOCHIP1_OFFSETS;


// GPIOCHIP2
typedef enum {
    GPIO_5_OFFSET  = 15,
    GPIO_6_OFFSET  = 17,
    GPIO_12_OFFSET = 16,
    GPIO_13_OFFSET = 18,
    GPIO_14_OFFSET = 14,
    GPIO_15_OFFSET = 13,
    GPIO_16_OFFSET = 7,
    GPIO_17_OFFSET = 8,
    GPIO_18_OFFSET = 11,
    GPIO_19_OFFSET = 12,
    GPIO_20_OFFSET = 10,
    GPIO_21_OFFSET = 9

} GPIOCHIP2_OFFSETS;


int gpio_init( const char* chip_path , uint64_t* offsets , int offset_count);
int gpio_read( uint64_t offset );
int gpio_close( void );

#endif
