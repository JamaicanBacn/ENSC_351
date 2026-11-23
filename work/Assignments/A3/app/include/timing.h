#ifndef TIMING_H
#define TIMING_H
enum TimingSource { AUDIO_REFILL, ACCEL_READ };
void timing_mark(enum TimingSource source);
void timing_report(void);
#endif
