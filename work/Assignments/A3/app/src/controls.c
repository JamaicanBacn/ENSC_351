#include "controls.h"
#include "beats.h"
#include <stdatomic.h>
#include <stdlib.h>

static atomic_bool stopping;

static long long nowMs(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000LL + t.tv_nsec / 1000000;
}

static int clamp(int value, int low, int high)
{
    if (value < low) return low;
    if (value > high) return high;
    return value;
}

void *controls_run(void *arg)
{
    Controls *c = arg;
    int tempo = 120;
    BEATS mode = ROCK;
    bool pressed = false, stablePress = false;
    int previousDirection = 0;
    long long edgeTime = 0, repeatAt = 0, sampleAt = 0;
    long long drumReady[3] = {0};
    const int drum[3] = {DRUM_SNARE_HARD, DRUM_CH, DRUM_BD_HARD};
    read_Accel(&c->accel);
    double baseline[3] = {c->accel.x, c->accel.y, c->accel.z};

    while (!atomic_load(&stopping)) {
        long long now = nowMs();
        int previousPosition = c->rotary.position;
        read_rotary(&c->rotary);
        int steps = c->rotary.position - previousPosition;
        if (steps) {
            tempo = clamp(tempo + steps * 5, 40, 300);
            changeBpm(tempo);
        }

        bool down = !c->rotary.sel;
        if (down != pressed) {
            pressed = down;
            edgeTime = now;
        }
        if (pressed != stablePress && now - edgeTime >= 25) {
            stablePress = pressed;
            if (stablePress) {
                mode = (mode + 1) % BEAT_COUNT;
                changeBeat(mode);
            }
        }

        if (now >= sampleAt) {
            sampleAt = now + 10;
            JoyStick_Read(&c->joystick);
            int direction = 0;
            if (c->joystick.y_pos > 0.55f) direction = 1;
            if (c->joystick.y_pos < -0.55f) direction = -1;
            if (direction && (direction != previousDirection || now >= repeatAt)) {
                int volume = AudioMixer_getVolume();
                int next = clamp(volume + direction * 5, 0, 100);
                if (next != volume) AudioMixer_setVolume(next);
                repeatAt = now + (direction == previousDirection ? 180 : 350);
            }
            previousDirection = direction;

            read_Accel(&c->accel);
            int sample[3] = {c->accel.x, c->accel.y, c->accel.z};
            int strongest = -1, peak = 220;
            for (int axis = 0; axis < 3; axis++) {
                int delta = abs(sample[axis] - (int)baseline[axis]);
                baseline[axis] += (sample[axis] - baseline[axis]) / 8.0;
                if (delta > peak) {
                    peak = delta;
                    strongest = axis;
                }
            }
            if (strongest >= 0 && now >= drumReady[strongest]) {
                AudioMixer_queueSound(&sounds[drum[strongest]]);
                drumReady[strongest] = now + 150;
            }
        }
        const struct timespec delay = {0, 1000000};
        nanosleep(&delay, NULL);
    }
    return NULL;
}

void controls_stop(void)
{
    atomic_store(&stopping, true);
}
