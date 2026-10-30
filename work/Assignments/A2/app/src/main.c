
#include "gpio.h"
#include "rotaryEncoder.h"
#include "pwm.h"
#include "LED.h"
#include "m3208.h"


#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define PWM_BASE_PATH "/dev/hat/pwm/GPIO12/"

#define PWM_DUTY_PATH PWM_BASE_PATH"duty_cycle"
#define PWM_ENABLE_PATH PWM_BASE_PATH"enable"
#define PWM_PERIOD_PATH PWM_BASE_PATH"period"

#define SPI_PATH "/dev/spidev0.0"
#define SPI_SPEED 10000
#define SPI_BPT 8 // bits per transfer

#define VOLTS( reading ) ( 3.3 * (float)(reading) / 4096 )





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

uint64_t total_samples;
float second_samples[1000] = {0};

float average_intensity = 0;

uint64_t dips = 0;

bool dip_checker( int index);

void* light_sampler()
{

    struct timespec spec = {
        .tv_sec = 0,
        .tv_nsec = 1000000
    };

    float voltage;
    bool dip_active = false;

    while(1)
    {
        for( int i = 0; i < 1000 ; i++)
        {
            total_samples++;
            second_samples[i] = VOLTS( m_3208_read( M3208_CHANNEL_0));
            voltage = second_samples[i];

            average_intensity = average_intensity * 0.999 + 0.001 * voltage; 

            if( dip_checker(i) )
            {
                if( !dip_active )
                {
                    dip_active = true;
                    dips++;
                }
                
            }
            else if( voltage >= average_intensity - 0.07)
                {
                    dip_active = false;
                }

            printf("Voltage : %f , Average Intensity : %f Dips : %lu , Dip_Active : %d\n" , voltage , average_intensity, dips , dip_active );
            nanosleep(&spec , NULL);
        }
    }
}

int main()
{

    //make an array of the offsets for the rotary encoder
    uint32_t offsets[2] = { gpiochip2_offsets.GPIO_18_OFFSET, gpiochip2_offsets.GPIO_15_OFFSET};

    //Init the gpiochup structure
    GPIOCHIP gpio_chip_2 = {
        .gpio_path  = GPIOCHIP_2_PATH,
        .offsets    = offsets
    };



    gpio_init( &gpio_chip_2 , offsets , 2);
    spi_init( SPI_PATH , SPI_SPEED , SPI_BPT);

    Rotary_encoder R = init_Rotary_encoder( &gpio_chip_2 , offsets);
    PWM pwm_struct = pwm_init( PWM_DUTY_PATH , PWM_ENABLE_PATH , PWM_PERIOD_PATH);


    // Rotary encoder runs on a seperate thread to ensure correct readings
    pthread_t encoder , sampler;
    pthread_create( &encoder , NULL , rotary_handler , &R);
    pthread_create( &sampler , NULL , light_sampler , NULL );
    

    while(1)
    {
        control_speed( R.position , &pwm_struct);
    }

    return 0;
}

bool dip_checker( int index )
{
    if( index < 2)
    {
        return false;
    }

    bool dip_arr[3];

    dip_arr[0] = second_samples[index     ] <= average_intensity - 0.1;    
    dip_arr[1] = second_samples[index - 1 ] <= average_intensity - 0.1;
    dip_arr[2] = second_samples[index - 2 ] <= average_intensity - 0.1;

    return dip_arr[0] && dip_arr[1] && dip_arr[2]; 

}



