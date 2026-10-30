
#include "LED.h"
#include "pwm.h"





/*

    @control speed
        From the rotary encoder. position * 10 = speed in hz
    
    Functionality

        If control speed < 0 : default 10hz



*/
void control_speed( uint32_t control_speed , PWM* pwm_struct)
{

    pwm_struct->period = ONE_SECOND / ( 10 + control_speed);
    pwm_struct->duty_cycle = pwm_struct->period / 2;
    pwm_struct->enable = 1;

    pwm_set_all( *pwm_struct );

}