
#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <gpiod.h>

static struct gpiod_line *sel_line = NULL;

int gpio_init( struct gpiod_line *sel_line);
int gpio_read( int sel_line);

#endif
