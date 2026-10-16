
#ifndef I2C_H
#define I2C_H



#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define I2C_REGISTER_SET 0
#define I2C_PATH    "/dev/i2c-1"


int i2c_init( );


//Read from a I2c device that has registers
int i2c_read_register ( uint8_t slave_address  , uint8_t slave_register , uint8_t* buffer , ssize_t size );
// Read from a I2C device without registers
int i2c_read( uint8_t slave_address  , uint8_t* buffer , ssize_t size );

// Write from a I2C device , If the device has registers format buffer accordingly 
int i2c_write( uint8_t slave_address , uint8_t* buffer , ssize_t size);



#endif