#include <Arduino.h>
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library


#include <Adafruit_NeoPixel.h>
#include "USB.hpp"
#include "LED.hpp"


USB_PACKET packet = {0};
LED  led = {0};


void setup() {
  // put your setup code here, to run once:
  LED_INIT();
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if( usb_read( &packet , LED_PACKET_SIZE ) )
  {
    LED_packet_config( &packet , &led );

    //Serial.println(packet.head.START);
    //Serial.println(packet.head.CMD);
    //Serial.println(packet.head.SIZE);

    //Serial.println( led.ring);
    //Serial.println( led.rgb.R);
    //Serial.println( led.rgb.G);
    //Serial.println( led.rgb.B);

    
    set_Ledring( led );
    Serial.write(PACKET_PROCESSED);
   
  }


}

