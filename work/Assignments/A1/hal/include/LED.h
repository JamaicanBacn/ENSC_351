
#ifndef LED_H
#define LED_H

#include<stdio.h>
#include<fcntl.h>

FILE* init_led(char * led_brightness , char* led_trigger);
int write_to_led( FILE* led_path , char* value);
int close_led(FILE* led_path);

#endif