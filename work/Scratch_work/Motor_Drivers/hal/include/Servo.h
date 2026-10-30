
#ifndef SERVO_H
#define SERVO_H

#include "i2c.h"
#include <stdlib.h>
#include<stdio.h>

#define SERVO_I2C_ADDR 0x40 // default adress

typedef enum{
    Servo_R1
} Servo_registers;



int servo_init( char* i2c_path);
void set_servo_angle( uint8_t servo  , /*degrees*/ uint8_t angle );


#endif