

#ifndef IR_REC_H
#define IR_REC_H

#include "gpio.h"


int init_IR_REC( const char* chip_path , unsigned int offset);
int read_IR_REC( void );

#endif