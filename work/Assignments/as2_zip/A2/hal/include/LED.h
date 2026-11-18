
#ifndef LED_H
#define LED_H

#include "pwm.h"
#include<stdio.h>
#include<stdint.h>
#include<fcntl.h>


void control_speed( uint32_t control_speed , PWM* pwm_struct);

#endif