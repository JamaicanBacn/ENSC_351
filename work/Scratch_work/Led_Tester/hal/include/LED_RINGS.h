
#ifndef LED_RINGS_H
#define LED_RINGS_H

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "USB.h"

#define RING_COUNT 1

#define LED_SIZE sizeof(LED)
#define LED_PACKET_SIZE sizeof(LED_PACKET) 
#define LED_USB_CMD     0x00
#define LED_START       0xAA

// Some base colours for RGB STRUCT
#define RED     (RGB){ 255 , 0   , 0     }
#define GREEN   (RGB){ 0   , 255 , 0     }
#define BLUE    (RGB){ 0   , 0   , 255   }


#define LED_PRINT_FORMAT " fd : %d\n USB_START : %d\n USB_CMD : %d\n USB_SIZE : %d\n RING : %d\n RED : %d\n GREEN :  %d\n BLUE : %d\n"


/*
    Sending For LED_RING

    Methodology
        Each led on a ring will have the same colour
        Hence when sending data each ring will be updated
    
    Transmission
        PACKET
            | RING_NUMBER | RED | GREEN | BLUE |

    @RING_NUMBER
        Which ring the data is intended for
    
    @RGB
        Controls the colour of the RING
        

*/

// Contains the wrie information for the LED

typedef struct 
{

    uint8_t R;
    uint8_t G;
    uint8_t B;

} RGB;

typedef struct 
{
    uint8_t RING_NUMBER;
    RGB rgb;

} LED;

// Data that will be written to usb
typedef struct
{
    HEADER head;
    LED led;

} LED_PACKET;

// struct for calling LED USB functions
typedef struct
{
    LED_PACKET packet;
    int usb_fd;

} LED_STRUCT;

void LED_write_debug(LED_STRUCT *led_s);
void print_led_packet( LED_STRUCT LED_S );
int LED_set_colour( LED_STRUCT led_s , uint8_t ring , RGB rgb);
LED_STRUCT LED_init( char* usb_path );
int LED_write( LED_STRUCT led_struct);


#endif