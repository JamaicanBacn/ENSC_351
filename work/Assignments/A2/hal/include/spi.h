
#ifndef SPI_H
#define SPI_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>



int spi_init(char* spi_path , uint32_t speed , uint32_t bits_per_word);
int spi_transfer( uint8_t* tx , uint8_t * rx , size_t len);
void spi_close(void);

#endif