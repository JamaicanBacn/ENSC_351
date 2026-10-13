
#include "LED_RINGS.h"

void LED_write_debug(LED_STRUCT *led_s)
{
    printf("HEADER size: %zu\n", sizeof(HEADER));
    printf("LED size: %zu\n", sizeof(LED));
    printf("Actual packet size: %zu\n",
           sizeof(led_s->packet));

    printf("Expected packet size: %ld\n",
           LED_PACKET_SIZE);

    uint8_t *data = (uint8_t *)&led_s->packet;

    printf("Bytes: ");

    for (size_t i = 0; i < sizeof(led_s->packet); i++) {
        printf("%02X ", data[i]);
    }

    printf("\n");
}

LED_STRUCT LED_init( char* usb_path)
{
    LED_STRUCT led_struct = {0};


    led_struct.usb_fd = USB_init( usb_path );
    led_struct.packet.head = (HEADER){LED_START, LED_USB_CMD , LED_SIZE};

    sleep(2);

    return led_struct;

}

int LED_set_colour( LED_STRUCT led_s , uint8_t ring , RGB rgb)
{
    led_s.packet.led.RING_NUMBER = ring;
    led_s.packet.led.rgb = rgb;
    return LED_write( led_s );
}

int LED_write( LED_STRUCT LED_S )
{

    LED_write_debug( &LED_S );
    ssize_t bytes = USB_write( LED_S.usb_fd , &LED_S.packet , LED_PACKET_SIZE);

    while( USB_read( LED_S.usb_fd) != USB_HANDSHAKE );

    return bytes;

}

void print_led_packet( LED_STRUCT led_s )
{
    HEADER head = led_s.packet.head;
    LED led     = led_s.packet.led;
    RGB rgb     = led.rgb;

    printf( LED_PRINT_FORMAT , led_s.usb_fd , head.START , head.COMMAND , head.SIZE , led.RING_NUMBER , rgb.R , rgb.G , rgb.B );
}