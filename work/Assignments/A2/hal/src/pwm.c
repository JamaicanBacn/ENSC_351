

#include "pwm.h"
#include <stdio.h>

PWM pwm_init( char* duty_path , char* enable_path , char* period_path)
{
    PWM pwm_struct = {
        .duty_fd    = fopen(duty_path , "w"),
        .enable_fd  = fopen(enable_path , "w"),
        .period_fd  = fopen(period_path , "w"),

        .duty_cycle = 0,
        .enable = 0,
        .period = 0
    };


    if( pwm_struct.duty_fd < 0 )
    {
        perror("PWM DUTY FD FAILED");
        exit(1);
    }

    if( pwm_struct.enable_fd < 0 )
    {
        perror("PWM ENABLE FD FAILED");
        exit(1);
    }

    if( pwm_struct.period_fd < 0 )
    {
        perror("PWM PERODP FD FAILED");
        exit(1);
    }

    return pwm_struct;
}

void pwm_set_duty( PWM pwm_struct)
{
    if( fseek(pwm_struct.duty_fd , 0 , SEEK_SET) != 0)
    {
        perror("FSEEK FAILED DUTY CYCLE PWM");
        exit(1);
    }

    if( fprintf( pwm_struct.duty_fd , "%u\n" , pwm_struct.duty_cycle) < 0)
    {
        perror("FPRINTF FAILED PWM DUTY");
        exit(1);
    }

    if( fflush(pwm_struct.duty_fd) == EOF)
    {
        perro("FFLUSH FAILED PWM DUTY");
        exit(1);
    }


}

void pwm_set_enable( PWM pwm_struct)
{
    if( fseek(pwm_struct.enable_fd , 0 , SEEK_SET) != 0)
    {
        perror("FSEEK FAILED ENABLE PWM");
        exit(1);
    }

    if( fprintf( pwm_struct.enable_fd , "%u\n" , pwm_struct.enable) < 0)
    {
        perror("FPRINTF FAILED PWM ENABLE");
        exit(1);
    }

    if( fflush(pwm_struct.enable_fd) == EOF)
    {
        perro("FFLUSH FAILED PWM ENABLE");
        exit(1);
    }
}

void pwm_set_period( PWM pwm_struct)
{
    if( fseek(pwm_struct.period_fd , 0 , SEEK_SET) != 0)
    {
        perror("FSEEK FAILED PERIOD PWM");
        exit(1);
    }

    if( fprintf( pwm_struct.period_fd , "%u\n" , pwm_struct.period) < 0)
    {
        perror("FPRINTF FAILED PWM PERIOD");
        exit(1);
    }

    if( fflush(pwm_struct.period_fd) == EOF)
    {
        perro("FFLUSH FAILED PWM PERIOD");
        exit(1);
    }
}