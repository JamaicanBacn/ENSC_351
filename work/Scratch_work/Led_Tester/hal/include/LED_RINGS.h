
#ifndef LED_RINGS_H
#define LED_RINGS_H

#include <unistd.h>
#include <stdint.h>
#include "USB.h"

#define RING_COUNT 2

#define LED_PACKET_SIZE sizeof(LED_PACKET);
#define LED_USB_CMD     0x00

// Some base colours for RGB STRUCT
#define RED     { 255 , 0   , 0     }
#define BLUE    { 0   , 255 , 0     }
#define GREEN   { 0   , 0   , 255   }


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
    uint8_t RING_NUMBER;
    uint8_t R;
    uint8_t G;
    uint8_t B;

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
    int usb_fd

} LED_STRUCT;



LED_STRUCT LED_init( char* usb_path );
int LED_write( LED_STRUCT led_struct);


#endif