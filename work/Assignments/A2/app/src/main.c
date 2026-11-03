
#include "gpio.h"
#include "rotaryEncoder.h"
#include "pwm.h"
#include "LED.h"
#include "m3208.h"
#include "periodTimer.h"

#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#define PWM_BASE_PATH "/dev/hat/pwm/GPIO12/"

#define PWM_DUTY_PATH PWM_BASE_PATH"duty_cycle"
#define PWM_ENABLE_PATH PWM_BASE_PATH"enable"
#define PWM_PERIOD_PATH PWM_BASE_PATH"period"

#define SPI_PATH "/dev/spidev0.0"
#define SPI_SPEED 10000
#define SPI_BPT 8 // bits per transfer

#define PACKET_SIZE 1500
#define VOLTS( reading ) ( 3.3 * (float)(reading) / 4096 )
#define T_to_Hz( reading ) ( ONE_SECOND /  reading )

#define HELP "Accepted command examples:\ncount       -- get the total number of samples taken.\nlength      -- get the number of samples taken in the previously completed second.\ndips        -- get the number of dips in the previously completed second.\nhistory     -- get all the samples in the previously completed second.\nstop        -- cause the server program to end.\n<enter>     -- repeat last command.\n"

#define COUNT "# samples taken total: %lu\n"
#define LENGTH "samples taken last second: %d\n"
#define DIPS    " Dips: %lu\n"
#define STOP    "Terminating Program\n"

#define HISTORY_LINE "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f%f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n"
#define HISTORY_ELEMENT "%f, "


void terminal_display();

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

pthread_mutex_t sampler_mutex = PTHREAD_MUTEX_INITIALIZER;

PWM pwm_struct = {0};

uint8_t exit_cmd = 0;

uint64_t total_samples;
float second_samples[1000] = {0};

float average_intensity = 0;

uint64_t dips = 0;
uint64_t dips_in_last_second = 0;

Period_statistics_t pstats = {0};

bool dip_checker( int index);

void* light_sampler( void* exit)
{

    struct timespec spec = {
        .tv_sec = 0,
        .tv_nsec = 1000000
    };

    float voltage;
    bool dip_active = false;

    pthread_mutex_lock(&sampler_mutex);

    while(1)
    {
        unsigned long long current_time = time(NULL);
        unsigned long long end_time = current_time + 1; // one second later

        for( int i = 0; current_time < end_time && i < 1000 ; i++)
        {
            Period_markEvent(PERIOD_EVENT_SAMPLE_LIGHT);
            second_samples[i] = VOLTS( m_3208_read( M3208_CHANNEL_0));
            voltage = second_samples[i];
            total_samples++;

            average_intensity = average_intensity * 0.999 + 0.001 * voltage; 

            if( dip_checker(i) )
            {
                if( !dip_active )
                {
                    dip_active = true;
                    dips_in_last_second++;
                }
                
            }
            else if( voltage >= average_intensity - 0.07)
                {
                    dip_active = false;
                }

            nanosleep(&spec , NULL);
            current_time = time(NULL);
        }

        Period_getStatisticsAndClear(PERIOD_EVENT_SAMPLE_LIGHT , &pstats);
        terminal_display();

        pthread_mutex_unlock(&sampler_mutex);
        nanosleep(&spec , NULL);
        pthread_mutex_lock( &sampler_mutex);

        memset( second_samples , 0 , sizeof(second_samples));
        pthread_mutex_unlock(&sampler_mutex);

        if( exit_cmd == 1)
        {
            pthread_exit(0);
        }

    }
}

void* UDP_manager( )
{

    char prev_cmd[1000] = HELP;


    int local_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (local_socket == -1) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in server = {
        .sin_family = AF_INET,
        .sin_port = htons(12345),
        .sin_addr.s_addr = htonl(INADDR_ANY)
    };

    if (bind(local_socket, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("bind");
        close(local_socket);
        exit(1);
    }


    char buffer[PACKET_SIZE];
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while(1)
    {


        ssize_t bytes = recvfrom( local_socket , buffer , sizeof(buffer) - 1, 0 ,
                               (struct sockaddr*)&client_addr , &client_len);

        pthread_mutex_lock(&sampler_mutex);

        buffer[bytes] = '\0';
        buffer[strcspn(buffer, "\r\n")] = '\0';
        char response[PACKET_SIZE];
        
        if( !strcmp( buffer , "") )
        {
            memset( buffer , 0 , sizeof(buffer));
            strcpy(buffer , prev_cmd);
        }
        
        if( !strcmp(buffer , "help") || !strcmp(buffer , "?") )
        {
            snprintf(response, sizeof(response), HELP);

            sendto(
                local_socket,
                response,
                strlen(response),
                 0,
                 (struct sockaddr *)&client_addr,
                 client_len
            );

            strcpy(prev_cmd , buffer);
        }
        else if( !strcmp(buffer , "count") )
        {
            snprintf(response, sizeof(response),
                      COUNT, total_samples);

            sendto(
                local_socket,
                response,
                strlen(response),
                 0,
                 (struct sockaddr *)&client_addr,
                 client_len
            );

            strcpy(prev_cmd , buffer);

        }
        else if ( !strcmp(buffer , "length"))
        {
            snprintf(response, sizeof(response),
                      LENGTH , pstats.numSamples);

            sendto(
                local_socket,
                response,
                strlen(response),
                 0,
                 (struct sockaddr *)&client_addr,
                 client_len
            );

            strcpy(prev_cmd , buffer);

        }
        else if ( !strcmp(buffer , "history"))
        {
            int index = 0;

            for( index = 0; index < (int)(pstats.numSamples) -20 ; index += 20 )
            {
                snprintf(response, sizeof(response),
                    HISTORY_LINE,
                    second_samples[index],
                    second_samples[index + 1], 
                    second_samples[index + 2],
                    second_samples[index + 3],
                    second_samples[index + 4],
                    second_samples[index + 5],
                    second_samples[index + 6],
                    second_samples[index + 7],
                    second_samples[index + 8],
                    second_samples[index + 9],
                    second_samples[index + 10],
                    second_samples[index + 11], 
                    second_samples[index + 12],
                    second_samples[index + 13],
                    second_samples[index + 14],
                    second_samples[index + 15],
                    second_samples[index + 16],
                    second_samples[index + 17],
                    second_samples[index + 18],
                    second_samples[index + 19]   
                );

                sendto(
                local_socket,
                response,
                strlen(response),
                 0,
                 (struct sockaddr *)&client_addr,
                 client_len
                );
            }

            for( ; index < pstats.numSamples ; index++)
            {
                snprintf(response, sizeof(response),
                    HISTORY_ELEMENT , second_samples[index]);

                sendto(
                local_socket,
                response,
                strlen(response),
                 0,
                 (struct sockaddr *)&client_addr,
                 client_len
                );
                
            }

            strcpy(prev_cmd , buffer);
        }
        else if( !strcmp(buffer , "dips"))
        {
            snprintf(response, sizeof(response),
                      DIPS, dips_in_last_second);

            sendto(
                local_socket,
                response,
                strlen(response),
                 0,
                 (struct sockaddr *)&client_addr,
                 client_len
            );
            strcpy(prev_cmd , buffer);

        }
        else if( !strcmp( buffer , "stop"))
        {
            snprintf(response, sizeof(response),
                      STOP);



            sendto(
                local_socket,
                response,
                strlen(response),
                 0,
                 (struct sockaddr *)&client_addr,
                 client_len
            );

            exit_cmd = 1;
            pthread_exit(0);


        }

        pthread_mutex_unlock(&sampler_mutex);

    }
}

void terminal_display();
int main()
{

    //make an array of the offsets for the rotary encoder
    uint32_t offsets[2] = { gpiochip2_offsets.GPIO_18_OFFSET, gpiochip2_offsets.GPIO_15_OFFSET};

    //Init the gpiochup structure
    GPIOCHIP gpio_chip_2 = {
        .gpio_path  = GPIOCHIP_2_PATH,
        .offsets    = offsets
    };
    Period_init();

    gpio_init( &gpio_chip_2 , offsets , 2);
    spi_init( SPI_PATH , SPI_SPEED , SPI_BPT);

    Rotary_encoder R = init_Rotary_encoder( &gpio_chip_2 , offsets);
    R.exit = &exit_cmd;

    pwm_struct = pwm_init( PWM_DUTY_PATH , PWM_ENABLE_PATH , PWM_PERIOD_PATH);


    // Rotary encoder runs on a seperate thread to ensure correct readings
    pthread_t encoder , sampler , networking;

    
    pthread_create( &networking , NULL , UDP_manager , NULL);
    pthread_create( &encoder , NULL , rotary_handler , &R);
    pthread_create( &sampler , NULL , light_sampler , NULL );
    
    

    while( exit_cmd == 0 )
    {
        control_speed( R.position , &pwm_struct);
    }

    return 0;

    pthread_join( encoder , NULL);
    pthread_join( sampler , NULL);
    pthread_join( networking, NULL);
    return 0;

}
void terminal_display()
{
    

    printf( "#Smpl/s = %d    Flash @  %d   avg = %fV    dips =   %lu    Smpl ms[ %lf,  %lf] avg %lf/%d\n" ,
            pstats.numSamples ,
            T_to_Hz( pwm_struct.period),
            average_intensity ,
            dips_in_last_second,
            pstats.minPeriodInMs,
            pstats.maxPeriodInMs,
            pstats.avgPeriodInMs,
            pstats.numSamples);
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



