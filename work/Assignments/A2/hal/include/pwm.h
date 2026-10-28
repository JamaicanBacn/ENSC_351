
#ifndef PWM_H
#define PWM_H

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>

#define TEN_HZ 10000000




// Contains all relavent PWM values
typedef struct
{
    FILE* duty_fd;     
    FILE* enable_fd;
    FILE* period_fd;

    uint32_t duty_cycle;
    uint32_t enable;
    uint32_t period;

} PWM;

PWM pwm_init( char* duty_path , char* enable_path , char* period_path);

void pwm_set_all( PWM pwm_struct);
void pwm_set_duty( PWM pwm_struct);
void pwm_set_enable( PWM pwm_struct);
void pwm_set_period( PWM pwm_struct);



#endif