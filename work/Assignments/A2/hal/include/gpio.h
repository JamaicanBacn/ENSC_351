
#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <stdint.h>
#include <gpiod.h>

#define GPIOCHIP_0_PATH "/dev/gpiochip0"
#define GPIOCHIP_1_PATH "/dev/gpiochip1"
#define GPIOCHIP_2_PATH "/dev/gpiochip2"

typedef struct{
    char* gpio_path;
    struct gpiod_chip* chip;
    struct gpiod_line_request* request;

    uint32_t* offsets;

} GPIOCHIP;

#include <stdint.h>

// GPIOCHIP0
typedef struct {
    uint32_t GPIO_2_OFFSET;
    uint32_t GPIO_3_OFFSET;
    uint32_t GPIO_7_OFFSET;
    uint32_t GPIO_8_OFFSET;
    uint32_t GPIO_9_OFFSET;
    uint32_t GPIO_10_OFFSET;
    uint32_t GPIO_11_OFFSET;
    uint32_t GPIO_23_OFFSET;
    uint32_t GPIO_24_OFFSET;
} GPIOCHIP0_OFFSETS;


// GPIOCHIP1
typedef struct {
    uint32_t GPIO_4_OFFSET;
    uint32_t GPIO_22_OFFSET;
    uint32_t GPIO_25_OFFSET;
    uint32_t GPIO_26_OFFSET;
    uint32_t GPIO_27_OFFSET;
} GPIOCHIP1_OFFSETS;


// GPIOCHIP2
typedef struct {
    uint32_t GPIO_5_OFFSET;
    uint32_t GPIO_6_OFFSET;
    uint32_t GPIO_12_OFFSET;
    uint32_t GPIO_13_OFFSET;
    uint32_t GPIO_14_OFFSET;
    uint32_t GPIO_15_OFFSET;
    uint32_t GPIO_16_OFFSET;
    uint32_t GPIO_17_OFFSET;
    uint32_t GPIO_18_OFFSET;
    uint32_t GPIO_19_OFFSET;
    uint32_t GPIO_20_OFFSET;
    uint32_t GPIO_21_OFFSET;
} GPIOCHIP2_OFFSETS;


int gpio_init( GPIOCHIP* gp_chip, uint32_t* offsets , int offset_count);
int gpio_read( uint32_t offset , GPIOCHIP gp_chip );
int gpio_close( void );

#endif
