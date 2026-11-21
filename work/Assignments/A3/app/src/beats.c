#include "beats.h"

static long long getTimeInMs(void)
{
    struct timespec spec;
    clock_gettime( CLOCK_REALTIME, &spec);
    long long seconds = spec.tv_sec;
    long long nanoSeconds = spec.tv_nsec;
    long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;

    return milliSeconds;

}

static void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

wavedata_t sounds[SOUND_COUNT];

void* beatManager();
static bool beat_stopping;
static pthread_t beatThread;
static pthread_mutex_t beatMutex = PTHREAD_MUTEX_INITIALIZER;

BEATS beat;
int bpm;


void BeatsInit()
{
    beat_stopping = false;
    beat = NOTHING;
    int bpm = 120;

    for (enum SOUND_INDICES id = DRUM_BD_HARD; id < SOUND_COUNT; ++id) {
        AudioMixer_readWaveFileIntoMemory(wavPaths[id], &sounds[id]);
    }

    pthread_create( &beatThread , NULL , beatManager , NULL);
}

void* beatManager()
{
    while( !beat_stopping )
    {
        play( beat , bpm );
    }
}

void changeBpm( int new_bpm )
{
    pthread_mutex_lock(&beatMutex);
    bpm = new_bpm;
    pthread_mutex_unlock(&beatMutex);
}

void changeBeat( BEATS beats)
{
    pthread_mutex_lock(&beatMutex);
    beat = beats;
    pthread_mutex_unlock(&beatMutex);
}

void play( BEATS beats , int bpm)
{
    pthread_mutex_lock(&beatMutex);

    switch ( beats )
    {

        case ROCK    :
            Rock(bpm);
            break;
        case CUSTOM :
            Custom(bpm);
            break;
        default:
            break;
        
    }

    pthread_mutex_unlock(&beatMutex);
}


void Rock( int bpm )
{
    if (bpm <= 0) {
        return;
    }

    double MsDelay = (double) HALF_BEAT_MS / bpm;
    double sixteenthMs = MsDelay / 2.0;

    // Beat 1: two heavy kicks
    AudioMixer_queueSound(&sounds[DRUM_BD_HARD]);
    AudioMixer_queueSound(&sounds[DRUM_CH]);
    sleepForMs(MsDelay);

    AudioMixer_queueSound(&sounds[DRUM_BD_HARD]);
    sleepForMs(MsDelay);

    // Beat 2: hat alone, then a kick on the "&"
    AudioMixer_queueSound(&sounds[DRUM_CH]);
    sleepForMs(MsDelay);

    AudioMixer_queueSound(&sounds[DRUM_BD_HARD]);
    sleepForMs(MsDelay);

    // Beat 3: half-time snare, then leave space for it to ring
    AudioMixer_queueSound(&sounds[DRUM_SNARE_HARD]);
    AudioMixer_queueSound(&sounds[DRUM_CH]);
    sleepForMs(MsDelay * 2.0);

    // Beat 4: four rapid kicks leading back into beat 1
    AudioMixer_queueSound(&sounds[DRUM_BD_HARD]);
    AudioMixer_queueSound(&sounds[DRUM_CH]);
    sleepForMs(sixteenthMs);

    AudioMixer_queueSound(&sounds[DRUM_BD_HARD]);
    sleepForMs(sixteenthMs);

    AudioMixer_queueSound(&sounds[DRUM_BD_HARD]);
    sleepForMs(sixteenthMs);

    AudioMixer_queueSound(&sounds[DRUM_BD_HARD]);
    sleepForMs(sixteenthMs);
}

void Custom( int bpm )
{
    if (bpm <= 0) {
        return;
    }

    double MsDelay = (double) HALF_BEAT_MS / bpm;
    double sixteenthMs = MsDelay / 2.0;

    // Beat 1: kick, then offbeat hat
    AudioMixer_queueSound(&sounds[DRUM_BD_SOFT]);
    sleepForMs(MsDelay);

    AudioMixer_queueSound(&sounds[DRUM_CH]);
    sleepForMs(MsDelay);

    // Beat 2: kick and snare together, then offbeat hat
    AudioMixer_queueSound(&sounds[DRUM_BD_SOFT]);
    AudioMixer_queueSound(&sounds[DRUM_SNARE_HARD]);
    sleepForMs(MsDelay);

    AudioMixer_queueSound(&sounds[DRUM_CH]);
    sleepForMs(MsDelay);

    // Beat 3: kick, then offbeat hat
    AudioMixer_queueSound(&sounds[DRUM_BD_SOFT]);
    sleepForMs(MsDelay);

    AudioMixer_queueSound(&sounds[DRUM_CH]);
    sleepForMs(MsDelay);

    // Beat 4: kick and snare, then two quick hats
    AudioMixer_queueSound(&sounds[DRUM_BD_SOFT]);
    AudioMixer_queueSound(&sounds[DRUM_SNARE_HARD]);
    sleepForMs(MsDelay);

    AudioMixer_queueSound(&sounds[DRUM_CH]);
    sleepForMs(sixteenthMs);

    AudioMixer_queueSound(&sounds[DRUM_CH]);
    sleepForMs(sixteenthMs);
}

void beat_clean()
{
    beat_stopping = true;
    pthread_join(beatThread , NULL); 
}