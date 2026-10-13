#ifndef LED_H
#define LED_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <stdint.h>
#include "USB.hpp"


#define LED_RING_COUNT 1
#define LED_PER_RING 8
#define LED_PACKET_SIZE sizeof(HEADER) + sizeof(LED)

#define DELAYVAL

typedef struct 
{
  uint8_t R;
  uint8_t G;
  uint8_t B;
} RGB;

typedef struct
{
  uint8_t ring;
  RGB rgb;
} LED;

void LED_INIT();
void LED_packet_config( USB_PACKET* packet , LED* led);
void set_Ledring( LED led );

#endif