
#include "LED_RINGS.h"


LED_STRUCT LED_init( char* usb_path)
{
    LED_STRUCT led_struct = {0};


    led_struct.usb_fd = USB_init( usb_path );

    led_struct.packet.head.COMMAND = LED_USB_CMD;
    led_struct.packet.head.SIZE = sizeof(LED_PACKET);

    return led_struct;

}

int LED_write( LED_STRUCT LED_S )
{
    return USB_write( LED_S.usb_fd , &LED_S.packet , LED_S.packet.head.SIZE);
}