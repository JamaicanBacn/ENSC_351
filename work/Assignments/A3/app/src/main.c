#include    <stdint.h>
#include    <inttypes.h>
#include    "gpio.h"
#include    "spi.h"


#include    "accellerometer.h"
#include    "rotaryEncoder.h"
#include    "audioMixer.h"   
#include    "joystick.h"


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

#define ROTARY_GPIO       ((const unsigned int[]){ ROTARY_GPIO_VALUES })
#define OFFSETS           ((const unsigned int[]){ JOYSTICK_GPIO, ROTARY_GPIO_VALUES })

#define OFFSET_COUNT        4

#define DRUM_BD_HARD_PATH "wave-files/100051__menegass__gui-drum-bd-hard.wav"
#define DRUM_BD_SOFT_PATH "wave-files/100052__menegass__gui-drum-bd-soft.wav"
#define DRUM_CC_PATH "wave-files/100053__menegass__gui-drum-cc.wav"
#define DRUM_CH_PATH "wave-files/100054__menegass__gui-drum-ch.wav"
#define DRUM_CO_PATH "wave-files/100055__menegass__gui-drum-co.wav"
#define DRUM_CYN_HARD_PATH "wave-files/100056__menegass__gui-drum-cyn-hard.wav"
#define DRUM_CYN_SOFT_PATH "wave-files/100057__menegass__gui-drum-cyn-soft.wav"
#define DRUM_SNARE_HARD_PATH "wave-files/100058__menegass__gui-drum-snare-hard.wav"
#define DRUM_SNARE_SOFT_PATH "wave-files/100059__menegass__gui-drum-snare-soft.wav"
#define DRUM_SPLASH_HARD_PATH "wave-files/100060__menegass__gui-drum-splash-hard.wav"
#define DRUM_SPLASH_SOFT_PATH "wave-files/100061__menegass__gui-drum-splash-soft.wav"
#define DRUM_TOM_HI_HARD_PATH "wave-files/100062__menegass__gui-drum-tom-hi-hard.wav"
#define DRUM_TOM_HI_SOFT_PATH "wave-files/100063__menegass__gui-drum-tom-hi-soft.wav"
#define DRUM_TOM_LO_HARD_PATH "wave-files/100064__menegass__gui-drum-tom-lo-hard.wav"
#define DRUM_TOM_LO_SOFT_PATH "wave-files/100065__menegass__gui-drum-tom-lo-soft.wav"
#define DRUM_TOM_MID_HARD_PATH "wave-files/100066__menegass__gui-drum-tom-mid-hard.wav"
#define DRUM_TOM_MID_SOFT_PATH "wave-files/100067__menegass__gui-drum-tom-mid-soft.wav"

enum SOUND_INDICES
{
    DRUM_BD_HARD,
    DRUM_BD_SOFT,
    DRUM_CC,
    DRUM_CH,
    DRUM_CO,
    DRUM_CYN_HARD,
    DRUM_CYN_SOFT,
    DRUM_SNARE_HARD,
    DRUM_SNARE_SOFT,
    DRUM_SPLASH_HARD,
    DRUM_SPLASH_SOFT,
    DRUM_TOM_HI_HARD,
    DRUM_TOM_HI_SOFT,
    DRUM_TOM_LO_HARD,
    DRUM_TOM_LO_SOFT,
    DRUM_TOM_MID_HARD,
    DRUM_TOM_MID_SOFT,
    SOUND_COUNT
};

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

Accel_struct    accel;
Rotary_encoder  rotary;
JoyStick        joystick;

GPIOCHIP gp_chip2;

wavedata_t sounds[SOUND_COUNT] ;


void init_peripherals( GPIOCHIP* gp_chip , uint32_t offsets[] , int offset_count);
void print_peripherals();
void read_peripherals();

int main()
{

    unsigned int offsets[] = { JOYSTICK_GPIO, ROTARY_GPIO_VALUES };
    init_peripherals( &gp_chip2 , offsets , OFFSET_COUNT);

    
    AudioMixer_init();

    AudioMixer_readWaveFileIntoMemory(DRUM_BD_HARD_PATH , &sounds[DRUM_BD_HARD]);
    AudioMixer_readWaveFileIntoMemory(DRUM_BD_SOFT_PATH , &sounds[DRUM_BD_SOFT]);


    AudioMixer_cleanup();
    gpio_cleanup( &gp_chip2 );

    return 0;
}

void init_peripherals( GPIOCHIP* gp_chip , uint32_t offsets[] , int offset_count)
{
    gp_chip->gpio_path = GPIOCHIP_2_PATH;
    gp_chip->offsets = offsets;

    gpio_init( gp_chip , offsets , offset_count);
    spi_init( SPI_PATH , SPI_SPEED , SPI_BPT);
    accel       = init_Accellerometer(ACCEL_CHANNELS);
    rotary      = init_Rotary_encoder( gp_chip , ROTARY_GPIO);
    joystick    = JoyStick_Init( JOYSTICK_CHANNELS , gp_chip , JOYSTICK_GPIO);
}

void read_peripherals()
{
    read_Accel( &accel );
    read_rotary( &rotary);
    JoyStick_Read( &joystick);
    
}

void print_peripherals()
{

// Inside main(), after updating Accel, rotary, and joystick:
printf("Accel  [X=%4" PRIu32 " Y=%4" PRIu32 " Z=%4" PRIu32 "]  "
       "Rotary [Pos=%4d SEL=%d]  "
       "Joystick [X=%7.2f Y=%7.2f SEL=%d]\n",
       accel.x, accel.y, accel.z,
       rotary.position, (int)rotary.sel,
       joystick.x_pos, joystick.y_pos, (int)joystick.sel);
}

void print_terminal()
{

}