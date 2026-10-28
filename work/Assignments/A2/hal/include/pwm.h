
#ifndef PWM_H
#define PWM_H



typedef struct
{
    int duty_fd;
    int enable_fd;
    int period_fd;

    int duty_cycle;
    int enable;
    int period;

} PWM;

int pwm_init( char* duty_path , char* enable_path , char* period_path);

void pwm_set_duty( PWM pwm_struct);
void pwm_set_enable( PWM pwm_struct);
void pwm_set_period( PWM pwm_struct);



#endif