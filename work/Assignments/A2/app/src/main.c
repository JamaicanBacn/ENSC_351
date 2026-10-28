
#include "gpio.h"
#include "rotaryEncoder.h"
#include "pwm.h"
#include <pthread.h>

#define PWM_BASE_PATH "/dev/hat/pwm/GPIO12/"

#define PWM_DUTY_PATH PWM_BASE_PATH "duty_cycle"
#define PWM_ENABLE_PATH PWM_BASE_PATH "period"
#define PWM_PERIOD_PATH PWM_BASE_PATH "enable"


/*
    Structs contain the offsets for the specific gpiochip
*/
const GPIOCHIP0_OFFSETS gpiochip0_offsets = {
    .GPIO_2_OFFSET  = 18,
    .GPIO_3_OFFSET  = 17,
    .GPIO_7_OFFSET  = 9,
    .GPIO_8_OFFSET  = 0,
    .GPIO_9_OFFSET  = 4,
    .GPIO_10_OFFSET = 3,
    .GPIO_11_OFFSET = 2,
    .GPIO_23_OFFSET = 7,
    .GPIO_24_OFFSET = 10
};

const GPIOCHIP1_OFFSETS gpiochip1_offsets = {
    .GPIO_4_OFFSET  = 38,
    .GPIO_22_OFFSET = 41,
    .GPIO_25_OFFSET = 42,
    .GPIO_26_OFFSET = 36,
    .GPIO_27_OFFSET = 33
};

const GPIOCHIP2_OFFSETS gpiochip2_offsets = {
    .GPIO_5_OFFSET  = 15,
    .GPIO_6_OFFSET  = 17,
    .GPIO_12_OFFSET = 16,
    .GPIO_13_OFFSET = 18,
    .GPIO_14_OFFSET = 14,
    .GPIO_15_OFFSET = 13,
    .GPIO_16_OFFSET = 7,
    .GPIO_17_OFFSET = 8,
    .GPIO_18_OFFSET = 11,
    .GPIO_19_OFFSET = 12,
    .GPIO_20_OFFSET = 10,
    .GPIO_21_OFFSET = 9
};


int main()
{

    //make an array of the offsets for the rotary encoder
     uint32_t offsets[2] = { 
                            gpiochip2_offsets.GPIO_18_OFFSET,
                            gpiochip2_offsets.GPIO_15_OFFSET
                            };


    //Init the gpiochup structure
    GPIOCHIP gpio_chip_2 = {
        .gpio_path  = GPIOCHIP_2_PATH,
        .offsets    = offsets
    };



    gpio_init( &gpio_chip_2 , offsets , 2);

    Rotary_encoder R = init_Rotary_encoder( &gpio_chip_2 , offsets);
    PWM pwm_struct = pwm_init( PWM_DUTY_PATH , PWM_ENABLE_PATH , PWM_PERIOD_PATH);

    // Set the pwm struct values
    pwm_struct.duty_cycle = DUTY_PERCENT_50;
    pwm_struct.enable = 1;
    pwm_struct.period = 1;

    pwm_set_all(pwm_struct);

    // Rotary encoder runs on a seperate thread to ensure correct readings
    pthread_t encoder;
    pthread_create( &encoder , NULL , rotary_handler , &R);

    while(1)
    {
        printf( "pos : %d , Clk : %u , DT : %u , STATE : %u\n" , R.position, R.clk , R.DT , R.prev_state);
    }

    return 0;
}





