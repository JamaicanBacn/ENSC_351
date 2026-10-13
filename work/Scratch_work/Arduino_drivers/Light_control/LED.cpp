
#include "LED.hpp"

#define PIN 6
Adafruit_NeoPixel pixels(LED_PER_RING, PIN, NEO_GRB + NEO_KHZ800);

void LED_INIT()
{
  pixels.begin();
  pixels.clear();
  pixels.show();
}

void LED_packet_config( USB_PACKET* packet , LED* led)
{

  led->ring = packet->data[0];
  led->rgb.R = packet->data[1];
  led->rgb.G = packet->data[2];
  led->rgb.B = packet->data[3];

}

void set_Ledring( LED led )
{
  int ring_offset = led.ring * LED_PER_RING;

  for(int led_offset = 0; led_offset < LED_PER_RING; led_offset++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor( ring_offset + led_offset,
                          pixels.Color(led.rgb.R , led.rgb.G , led.rgb.B));

  }

  pixels.show();
}