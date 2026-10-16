

#include "i2c.h"


static int i2c_fd;

int i2c_init()
{

    int i2c_fd = open( I2C_PATH , O_RDWR);


    if( i2c_fd < 0 )
    {
        perror("I2C opening failed");
        return -1;
    }

    return i2c_fd;

}

int i2c_read_register( uint8_t slave_address , uint8_t slave_reg , uint8_t* buffer , ssize_t size )
{
    // First creat the msg struct 

    struct i2c_msg msg[2] = {
        {
            .addr   = slave_address,
            .flags  = I2C_REGISTER_SET,
            .buf    = &slave_reg,
            .len    = 1
        },
        {
            .addr   = slave_address,
            .flags  = I2C_M_RD,
            .buf    = buffer,
            .len    = size
        }
    };


    struct i2c_rdwr_ioctl_data transfer = {
        .msgs = &msg,
        .nmsgs = 2,
    };


    if( ioctl( i2c_fd , I2C_RDWR , &transfer ) == -1 )
    {
        perror("I2C READ - READ FAILED ");
        return -1;
    }

    return 1;

}

int i2c_read(  uint8_t slave_address , uint8_t* buffer , ssize_t size )
{
    struct i2c_msg msg = {
        .addr   = slave_address,
        .flags  = I2C_M_RD,
        .buf    = buffer,
        .len    = size
    };

    struct i2c_rdwr_ioctl_data transfer = {
        .msgs = &msg,
        .nmsgs = 1
    };

    if( ioctl( i2c_fd , I2C_RDWR , &transfer) == -1 )
    {
        perror("I2C READ - READ FAILED");
        return -1;
    }

    return 1;

}

int i2c_write( uint8_t slave_address , uint8_t* buffer , ssize_t size)
{


    // Format the message for the I2C buffer
    
    struct i2c_msg msg = {
        .addr   = slave_address,
        .buf    = buffer,
        .flags  = I2C_REGISTER_SET,
        .len    = size
    };

    // Pack into the I2C transfer struct
    struct i2c_rdwr_ioctl_data transfer = {
        .msgs   = &msg,
        .nmsgs  = 1
    };


    // check if the i2c transfer failed
    if( ioctl( i2c_fd , I2C_RDWR , &transfer ) < 0 )
    {
        perror("I2C WRITE FAILED");
        return -1;
    }

    return 1;
        
}

