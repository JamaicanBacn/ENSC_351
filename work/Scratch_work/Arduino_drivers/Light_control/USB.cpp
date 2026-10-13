
#include "USB.hpp"


int usb_read( USB_PACKET* packet , uint8_t packet_size)
{
  HEADER head; 

    if( Serial.available() < packet_size)
    {
      return NO_PACKET;
    }

    head.START  = Serial.read();

    if( head.START != USB_START )
    {
      return NO_PACKET;
    }

    head.CMD   = Serial.read();
    head.SIZE    = Serial.read();

    for( int i = 0; i < head.SIZE ; i++)
    {
      packet->data[i] = Serial.read();
    }

    packet->head = head;


    return PACKET_RECEIVED;


}