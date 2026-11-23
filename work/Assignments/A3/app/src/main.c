#include    <stdint.h>
#include    <signal.h>
#include    "controls.h"

#include    "gpio.h"
#include    "spi.h"
#include    "accellerometer.h"
#include    "rotaryEncoder.h"
#include    "audioMixer.h"   
#include    "joystick.h"
#include    "beats.h"


#define SPI_PATH "/dev/spidev0.0"
#define SPI_SPEED 10000
#define SPI_BPT 8 // bits per transfer

#define ACCEL_CHANNELS    ((uint8_t[]){ 5 , 6 , 7 })
#define JOYSTICK_CHANNELS ((uint8_t[]){ 0, 1 })

#define JOYSTICK_GPIO     (gpiochip2_offsets.GPIO_17_OFFSET)
#define ROTARY_DT_GPIO    (gpiochip2_offsets.GPIO_18_OFFSET)
#define ROTARY_CLK_GPIO   (gpiochip2_offsets.GPIO_15_OFFSET)
#define ROTARY_SEL_GPIO   (gpiochip2_offsets.GPIO_14_OFFSET)

#define ROTARY_GPIO_VALUES ROTARY_DT_GPIO, ROTARY_CLK_GPIO, ROTARY_SEL_GPIO

#define ROTARY_GPIO       ((unsigned int[]){ ROTARY_GPIO_VALUES })
#define OFFSETS           ((const unsigned int[]){ JOYSTICK_GPIO, ROTARY_GPIO_VALUES })

#define OFFSET_COUNT        4



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

int main(void)
{
    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGINT);
    sigaddset(&signals, SIGTERM);
    if (pthread_sigmask(SIG_BLOCK, &signals, NULL)) return EXIT_FAILURE;

    GPIOCHIP chip = {.gpio_path = GPIOCHIP_2_PATH};
    unsigned int offsets[] = {JOYSTICK_GPIO, ROTARY_GPIO_VALUES};
    chip.offsets = offsets;
    if (gpio_init(&chip, offsets, OFFSET_COUNT) < 0) return EXIT_FAILURE;
    if (spi_init(SPI_PATH, SPI_SPEED, SPI_BPT) < 0) {
        perror("SPI initialization");
        spi_close();
        gpio_cleanup(&chip);
        return EXIT_FAILURE;
    }

    Controls controls = {
        .accel = init_Accellerometer(ACCEL_CHANNELS),
        .rotary = init_Rotary_encoder(&chip, ROTARY_GPIO),
        .joystick = JoyStick_Init(JOYSTICK_CHANNELS, &chip, JOYSTICK_GPIO)
    };
    AudioMixer_init();
    BeatsInit();
    changeBeat(ROCK);

    pthread_t inputThread;
    int error = pthread_create(&inputThread, NULL, controls_run, &controls);
    if (error == 0) {
        int received;
        error = sigwait(&signals, &received);
        controls_stop();
        pthread_join(inputThread, NULL);
    }
    beat_clean();
    AudioMixer_cleanup();
    for (int i = 0; i < SOUND_COUNT; i++) AudioMixer_freeWaveFileData(&sounds[i]);
    spi_close();
    gpio_cleanup(&chip);
    if (error) fprintf(stderr, "Input control error: %s\n", strerror(error));
    return error ? EXIT_FAILURE : EXIT_SUCCESS;
}
