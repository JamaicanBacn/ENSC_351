

#include "LED_RINGS.h"
#include "IR_receiver.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define USB0_path "/dev/ttyUSB0"

#define GPIO_PATH   "/dev/gpiochip2"
#define GPIO_OFFSET 11


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

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static pthread_mutex_t led_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int offset;
    int ring;
    GPIOCHIP *gpio;
    LED_STRUCT *led;
} RING_THREAD_ARGS;

static void *ring_thread(void *arg)
{
    RING_THREAD_ARGS *args = arg;
    int previous_detected = -1;

    while (1) {
        int detected = read_IR_REC(args->offset, args->gpio);

        // Send only when the colour needs to change.
        if (detected != previous_detected) {
            pthread_mutex_lock(&led_mutex);

            if (detected) {
                LED_set_colour(*args->led, args->ring, RED);
            } else {
                LED_set_colour(*args->led, args->ring, BLUE);
            }

            pthread_mutex_unlock(&led_mutex);
            previous_detected = detected;
        }

        if (detected) {
            // Only this ring's thread pauses.
            usleep(500000);
        } else {
            usleep(1000);
        }
    }

    return NULL;
}

int main()
{
 //make an array of the offsets for the rotary encoder
    uint32_t offsets[3] = { gpiochip2_offsets.GPIO_18_OFFSET,
                            gpiochip2_offsets.GPIO_15_OFFSET,
                            gpiochip2_offsets.GPIO_14_OFFSET};

    //Init the gpiochup structure
    GPIOCHIP gpio_chip_2 = {
        .gpio_path  = GPIOCHIP_2_PATH,
        .offsets    = offsets
    };
    gpio_init( &gpio_chip_2 , offsets , 3);
     
    LED_STRUCT test_struct = LED_init( USB0_path );

    if( test_struct.usb_fd == -1)
    {
        perror("LED_INIT FAILED");
        exit(1);
    }

    /*

    while( 1 )
    {
        
        if( read_IR_REC( offsets[0] , &gpio_chip_2) )
        {
            LED_set_colour(test_struct , 0 , RED);
            usleep(500000);
        }
        else{
            LED_set_colour(test_struct , 0 , BLUE);
        }

        if( read_IR_REC( offsets[1] , &gpio_chip_2) )
        {
            LED_set_colour(test_struct , 1 , RED);
            usleep(500000);
        }
        else{
            LED_set_colour(test_struct , 1 , BLUE);
        }

        if( read_IR_REC( offsets[2] , &gpio_chip_2) )
        {
            LED_set_colour(test_struct , 2 , RED);
            usleep(500000);
        }
        else{
            LED_set_colour(test_struct , 2 , BLUE);
        }

        


    }
    
    */
    
    /*
    
    for( int i = 0; i < 100 ; i++ )
    {

        LED_set_colour( test_struct , 0 , RED);
        usleep(50000);
        LED_set_colour( test_struct , 0 , GREEN);
        usleep(50000);
        LED_set_colour( test_struct , 0 , BLUE);
        usleep(50000);

    }

    */
    
    pthread_t threads[3];
    RING_THREAD_ARGS args[3];

for (int i = 0; i < 3; i++) {
    args[i].offset = offsets[i];
    args[i].ring = i;
    args[i].gpio = &gpio_chip_2;
    args[i].led = &test_struct;

    int result = pthread_create(
        &threads[i],
        NULL,
        ring_thread,
        &args[i]
    );

    if (result != 0) {
        fprintf(stderr, "pthread_create: %s\n", strerror(result));
        return 1;
    }
}

// Keep main alive while the three threads run.
for (int i = 0; i < 3; i++) {
    pthread_join(threads[i], NULL);
}

    sleep(2);


    return 0;
}