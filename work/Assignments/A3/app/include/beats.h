
#ifndef BEATS_H
#define BEATS_H

#include<time.h>
#include<pthread.h>
#include<stdbool.h>

#include "audioMixer.h"


#define DRUM_BD_HARD_PATH "beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav"
#define DRUM_BD_SOFT_PATH "beatbox-wav-files/100052__menegass__gui-drum-bd-soft.wav"
#define DRUM_CC_PATH "beatbox-wav-files/100053__menegass__gui-drum-cc.wav"
#define DRUM_CH_PATH "beatbox-wav-files/100054__menegass__gui-drum-ch.wav"
#define DRUM_CO_PATH "beatbox-wav-files/100055__menegass__gui-drum-co.wav"
#define DRUM_CYN_HARD_PATH "beatbox-wav-files/100056__menegass__gui-drum-cyn-hard.wav"
#define DRUM_CYN_SOFT_PATH "beatbox-wav-files/100057__menegass__gui-drum-cyn-soft.wav"
#define DRUM_SNARE_HARD_PATH "beatbox-wav-files/100058__menegass__gui-drum-snare-hard.wav"
#define DRUM_SNARE_SOFT_PATH "beatbox-wav-files/100059__menegass__gui-drum-snare-soft.wav"
#define DRUM_SPLASH_HARD_PATH "beatbox-wav-files/100060__menegass__gui-drum-splash-hard.wav"
#define DRUM_SPLASH_SOFT_PATH "beatbox-wav-files/100061__menegass__gui-drum-splash-soft.wav"
#define DRUM_TOM_HI_HARD_PATH "beatbox-wav-files/100062__menegass__gui-drum-tom-hi-hard.wav"
#define DRUM_TOM_HI_SOFT_PATH "beatbox-wav-files/100063__menegass__gui-drum-tom-hi-soft.wav"
#define DRUM_TOM_LO_HARD_PATH "beatbox-wav-files/100064__menegass__gui-drum-tom-lo-hard.wav"
#define DRUM_TOM_LO_SOFT_PATH "beatbox-wav-files/100065__menegass__gui-drum-tom-lo-soft.wav"
#define DRUM_TOM_MID_HARD_PATH "beatbox-wav-files/100066__menegass__gui-drum-tom-mid-hard.wav"
#define DRUM_TOM_MID_SOFT_PATH "beatbox-wav-files/100067__menegass__gui-drum-tom-mid-soft.wav"

#define HALF_BEAT_MS 30000

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

typedef enum BEATS
{
    NOTHING,
    ROCK,
    CUSTOM,
    BEAT_COUNT

} BEATS;

extern wavedata_t sounds[SOUND_COUNT];

static char *const wavPaths[SOUND_COUNT] = {
    [DRUM_BD_HARD]      = DRUM_BD_HARD_PATH,
    [DRUM_BD_SOFT]      = DRUM_BD_SOFT_PATH,
    [DRUM_CC]           = DRUM_CC_PATH,
    [DRUM_CH]           = DRUM_CH_PATH,
    [DRUM_CO]           = DRUM_CO_PATH,
    [DRUM_CYN_HARD]     = DRUM_CYN_HARD_PATH,
    [DRUM_CYN_SOFT]     = DRUM_CYN_SOFT_PATH,
    [DRUM_SNARE_HARD]   = DRUM_SNARE_HARD_PATH,
    [DRUM_SNARE_SOFT]   = DRUM_SNARE_SOFT_PATH,
    [DRUM_SPLASH_HARD]  = DRUM_SPLASH_HARD_PATH,
    [DRUM_SPLASH_SOFT]  = DRUM_SPLASH_SOFT_PATH,
    [DRUM_TOM_HI_HARD]  = DRUM_TOM_HI_HARD_PATH,
    [DRUM_TOM_HI_SOFT]  = DRUM_TOM_HI_SOFT_PATH,
    [DRUM_TOM_LO_HARD]  = DRUM_TOM_LO_HARD_PATH,
    [DRUM_TOM_LO_SOFT]  = DRUM_TOM_LO_SOFT_PATH,
    [DRUM_TOM_MID_HARD] = DRUM_TOM_MID_HARD_PATH,
    [DRUM_TOM_MID_SOFT] = DRUM_TOM_MID_SOFT_PATH
};


void BeatsInit();
void changeBeat(BEATS beat);
void changeBpm(int new_bpm);
int getBpm(void);
BEATS getBeat(void);
BEATS nextBeat(void);
void adjustBpm(int delta);
void beat_clean();

#endif