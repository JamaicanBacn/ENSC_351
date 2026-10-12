
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "USB.h"


int USB_init( char* usb_path)
{
    int fd = open( usb_path , O_RDWR | O_NOCTTY);


    // check if the usb fd suceeded

    if( fd < 0 )
    {
        perror("USB failed to open");
        return -1;
    }

    struct termios tty;

    // Reads the current serial settings
    if(tcgetattr(fd,&tty) < 0)
    {
        perror("tcgetattr FAILED");
        close(fd);
        return -1;
    }

    // Disables from processing terminal characters
    cfmakeraw(&tty);

    // Set baud rate
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // 8 data bits
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    // No parity, one stop bit
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;

    // Enable receiving, ignore modem control lines
    tty.c_cflag |= CLOCAL | CREAD;

    // Disable software flow control
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Disable hardware flow control
#ifdef CRTSCTS
    tty.c_cflag &= ~CRTSCTS;
#endif

    // Configure read() behavior
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 10;

    // Apply settings
    if (tcsetattr(fd, TCSANOW, &tty) < 0) {
        perror("tcsetattr");
        close(fd);
        return -1;
    }

    return fd;

}

int USB_write( int usb_fd  , void* packet , int packet_size)
{
    return write( usb_fd , packet , packet_size );
}