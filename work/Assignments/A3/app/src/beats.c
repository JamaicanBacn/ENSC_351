#include "beats.h"

#include <errno.h>

#define BEAT_POLL_MS 10
#define NS_PER_MS 1000000LL
#define NS_PER_SECOND 1000000000LL

wavedata_t sounds[SOUND_COUNT];

static void* beatManager(void* arg);
static void play(BEATS beats, int bpm);
static void Rock(int bpm);
static void Custom(int bpm);

static bool beat_stopping;
static pthread_t beatThread;
static pthread_mutex_t beatMutex = PTHREAD_MUTEX_INITIALIZER;
static BEATS beat;
static int bpm;

static bool waitForBeat(double delayMs, BEATS expectedBeat)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    long long endNs = now.tv_sec * NS_PER_SECOND + now.tv_nsec
        + (long long)(delayMs * NS_PER_MS);

    while (true)
    {
        pthread_mutex_lock(&beatMutex);
        bool keepPlaying = !beat_stopping && beat == expectedBeat;
        pthread_mutex_unlock(&beatMutex);

        if (!keepPlaying)
        {
            return false;
        }

        clock_gettime(CLOCK_MONOTONIC, &now);
        long long nowNs = now.tv_sec * NS_PER_SECOND + now.tv_nsec;
        if (nowNs >= endNs)
        {
            return true;
        }

        // Limit each sleep without adding polling overhead to the note delay.
        long long wakeNs = nowNs + BEAT_POLL_MS * NS_PER_MS;
        if (wakeNs > endNs)
        {
            wakeNs = endNs;
        }
        struct timespec wake = {wakeNs / NS_PER_SECOND, wakeNs % NS_PER_SECOND};
        int result = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &wake, NULL);
        if (result != 0 && result != EINTR)
        {
            return false;
        }
    }
}

void BeatsInit()
{
    beat_stopping = false;
    beat = NOTHING;
    bpm = 120;

    for (enum SOUND_INDICES id = DRUM_BD_HARD; id < SOUND_COUNT; ++id) {
        AudioMixer_readWaveFileIntoMemory(wavPaths[id], &sounds[id]);
    }

    pthread_create( &beatThread , NULL , beatManager , NULL);
}

static void* beatManager(void* arg)
{
    (void)arg;

    while (true)
    {
        pthread_mutex_lock(&beatMutex);
        bool stopping = beat_stopping;
        BEATS local_beat = beat;
        int local_bpm = bpm;
        pthread_mutex_unlock(&beatMutex);

        if (stopping)
        {
            return NULL;
        }

        if (local_beat == NOTHING || local_bpm <= 0)
        {
            const struct timespec delay = {0, BEAT_POLL_MS * NS_PER_MS};
            nanosleep(&delay, NULL);
            continue;
        }

        play(local_beat, local_bpm);
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

static void play( BEATS beats , int bpm)
{

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

}


static void Rock( int bpm )
{
    if (bpm <= 0) {
        return;
    }

    double MsDelay = (double) HALF_BEAT_MS / bpm;
    double sixteenthMs = MsDelay / 2.0;

    // Beat 1: two heavy kicks
    AudioMixer_queueSound(&sounds[DRUM_BD_HARD]);
    AudioMixer_queueSound(&sounds[DRUM_CH]);
    if (!waitForBeat(MsDelay, ROCK)) {
        return;
    }

    AudioMixer_queueSound(&sounds[DRUM_BD_HARD]);
    if (!waitForBeat(MsDelay, ROCK)) {
        return;
    }

    // Beat 2: hat alone, then a kick on the "&"
    AudioMixer_queueSound(&sounds[DRUM_CH]);
    if (!waitForBeat(MsDelay, ROCK)) {
        return;
    }

    AudioMixer_queueSound(&sounds[DRUM_BD_HARD]);
    if (!waitForBeat(MsDelay, ROCK)) {
        return;
    }

    // Beat 3: half-time snare, then leave space for it to ring
    AudioMixer_queueSound(&sounds[DRUM_SNARE_HARD]);
    AudioMixer_queueSound(&sounds[DRUM_CH]);
    if (!waitForBeat(MsDelay * 2.0, ROCK)) {
        return;
    }

    // Beat 4: four rapid kicks leading back into beat 1
    AudioMixer_queueSound(&sounds[DRUM_BD_HARD]);
    AudioMixer_queueSound(&sounds[DRUM_CH]);
    if (!waitForBeat(sixteenthMs, ROCK)) {
        return;
    }

    AudioMixer_queueSound(&sounds[DRUM_BD_HARD]);
    if (!waitForBeat(sixteenthMs, ROCK)) {
        return;
    }

    AudioMixer_queueSound(&sounds[DRUM_BD_HARD]);
    if (!waitForBeat(sixteenthMs, ROCK)) {
        return;
    }

    AudioMixer_queueSound(&sounds[DRUM_BD_HARD]);
    if (!waitForBeat(sixteenthMs, ROCK)) {
        return;
    }
}

static void Custom( int bpm )
{
    if (bpm <= 0) {
        return;
    }

    double MsDelay = (double) HALF_BEAT_MS / bpm;
    double sixteenthMs = MsDelay / 2.0;

    // Beat 1: kick, then offbeat hat
    AudioMixer_queueSound(&sounds[DRUM_BD_SOFT]);
    if (!waitForBeat(MsDelay, CUSTOM)) {
        return;
    }

    AudioMixer_queueSound(&sounds[DRUM_CH]);
    if (!waitForBeat(MsDelay, CUSTOM)) {
        return;
    }

    // Beat 2: kick and snare together, then offbeat hat
    AudioMixer_queueSound(&sounds[DRUM_BD_SOFT]);
    AudioMixer_queueSound(&sounds[DRUM_SNARE_HARD]);
    if (!waitForBeat(MsDelay, CUSTOM)) {
        return;
    }

    AudioMixer_queueSound(&sounds[DRUM_CH]);
    if (!waitForBeat(MsDelay, CUSTOM)) {
        return;
    }

    // Beat 3: kick, then offbeat hat
    AudioMixer_queueSound(&sounds[DRUM_BD_SOFT]);
    if (!waitForBeat(MsDelay, CUSTOM)) {
        return;
    }

    AudioMixer_queueSound(&sounds[DRUM_CH]);
    if (!waitForBeat(MsDelay, CUSTOM)) {
        return;
    }

    // Beat 4: kick and snare, then two quick hats
    AudioMixer_queueSound(&sounds[DRUM_BD_SOFT]);
    AudioMixer_queueSound(&sounds[DRUM_SNARE_HARD]);
    if (!waitForBeat(MsDelay, CUSTOM)) {
        return;
    }

    AudioMixer_queueSound(&sounds[DRUM_CH]);
    if (!waitForBeat(sixteenthMs, CUSTOM)) {
        return;
    }

    AudioMixer_queueSound(&sounds[DRUM_CH]);
    if (!waitForBeat(sixteenthMs, CUSTOM)) {
        return;
    }
}

void beat_clean()
{
    pthread_mutex_lock(&beatMutex);
    beat_stopping = true;
    pthread_mutex_unlock(&beatMutex);
    pthread_join(beatThread , NULL);
}