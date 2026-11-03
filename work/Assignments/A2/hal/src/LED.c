
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

    uint32_t new_period = ONE_SECOND / (10 + control_speed);

    if (pwm_struct->enable &&
        pwm_struct->period == new_period) {
        return;
    }

    if( control_speed <= 0)
    {
        pwm_struct->enable = 0;
    }

    pwm_struct->period = new_period;
    pwm_struct->duty_cycle = new_period / 2;
    pwm_struct->enable = 1;
    pwm_set_all(*pwm_struct);

}