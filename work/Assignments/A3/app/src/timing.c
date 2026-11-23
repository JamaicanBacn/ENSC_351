#include "timing.h"
#include "beats.h"
#include <stdint.h>
#include <stdio.h>

struct Periods {
    double min, max, sum;
    unsigned count;
};
static struct Periods periods[2];
static int64_t previous[2];
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void timing_mark(enum TimingSource source)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    int64_t ns = (int64_t)now.tv_sec * 1000000000 + now.tv_nsec;
    pthread_mutex_lock(&lock);
    if (previous[source]) {
        double ms = (ns - previous[source]) / 1000000.0;
        struct Periods *p = &periods[source];
        if (!p->count || ms < p->min) p->min = ms;
        if (!p->count || ms > p->max) p->max = ms;
        p->sum += ms;
        p->count++;
    }
    previous[source] = ns;
    pthread_mutex_unlock(&lock);
}

void timing_report(void)
{
    pthread_mutex_lock(&lock);
    struct Periods audio = periods[AUDIO_REFILL], accel = periods[ACCEL_READ];
    periods[AUDIO_REFILL] = periods[ACCEL_READ] = (struct Periods){0};
    pthread_mutex_unlock(&lock);
    printf("M%d %dbpm vol:%d   Audio[%.3f, %.3f] avg %.3f/%u     Accel[%.3f, %.3f] avg %.3f/%u\n",
           getBeat(), getBpm(), AudioMixer_getVolume(),
           audio.min, audio.max, audio.count ? audio.sum / audio.count : 0, audio.count,
           accel.min, accel.max, accel.count ? accel.sum / accel.count : 0, accel.count);
    fflush(stdout);
}
