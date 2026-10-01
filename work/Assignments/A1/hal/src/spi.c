
#include "spi.h"

static int spi_fd = -1;
static uint32_t spi_speed;
static uint32_t spi_word_length;

int spi_init(char* spi_path , uint32_t speed , uint32_t bits_per_transfer )
{

    // Open spi file path 
    spi_fd = open(spi_path , O_RDWR);
    spi_speed = speed;
    spi_word_length = bits_per_transfer;


    // check if opening the file failed
    if( spi_fd < 0 )
    {
        return -1;
    }

    uint8_t mode = SPI_MODE_0;

    // Set the Write mode
    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &mode) < 0)
        return -1;

    // Set transfer bit length
    if (ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_transfer) < 0)
        return -1;

    // Set clock speed
    if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
        return -1;

    return 0;


}


int spi_transfer( uint8_t* tx , uint8_t * rx , size_t len)
{
    struct spi_ioc_transfer transfer = {
        .tx_buf = (unsigned long)tx, // transfer buffer
        .rx_buf = (unsigned long)rx, // receive buffer
        .len = len, // length in bytes
        .speed_hz = spi_speed, // clock speed
    };

    return ioctl(spi_fd , SPI_IOC_MESSAGE(1) , &transfer);
}

void spi_close(void)
{
    if( spi_fd >= 0)
    {
        close(spi_fd);
    }
}