#ifndef USB_H
#define USB_H

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>

#define USB_START 0xAA

#define NO_PACKET false
#define PACKET_RECEIVED true

#define PACKET_PROCESSED 0x11


// Define all the command types
#define LED_CMD 0x00


typedef struct 
{
  uint8_t START;
  uint8_t SIZE;
  uint8_t CMD;

} HEADER;

typedef struct
{
  HEADER head;
  uint8_t data[32];

} USB_PACKET;


int usb_read( USB_PACKET* packet , uint8_t packet_size);


#endif