

#ifndef USB_H
#define USB_H

#define MSG_START 0xAA

typedef struct
{
    uint8_t START;
    uint8_t SIZE;       // size of the packet in bytes
    uint8_t COMMAND;    // What the NEW MSG will be used for 

} HEADER;

typedef struct 
{
    HEADER HEAD;    // contains a info header for the arduino
    void* packet    // Memory locations of the packet 
} USB_PACKET;


int USB_init( char* usb_path);
int USB_write( int fd , void* packet , int packet_size);

#endif