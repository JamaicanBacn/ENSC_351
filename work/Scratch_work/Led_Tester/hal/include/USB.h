

#ifndef USB_H
#define USB_H

// The Arduino will send this back when the trasmission has been accepted
#define USB_HANDSHAKE 0x11

typedef struct
{
    uint8_t START;
    uint8_t COMMAND;       // size of the packet in bytes
    uint8_t SIZE;    // What the NEW MSG will be used for 

} HEADER;

typedef struct 
{
    HEADER HEAD;    // contains a info header for the arduino
    void* packet;    // Memory locations of the packet 
} USB_PACKET;


uint64_t USB_read( int usb_fd );
int USB_init( char* usb_path);
int USB_write( int fd , void* packet , int packet_size);

#endif