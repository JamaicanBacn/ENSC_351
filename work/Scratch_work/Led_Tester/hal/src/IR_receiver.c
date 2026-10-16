
#include "IR_receiver.h"
#include "time.h"


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

int init_IR_REC( const char* chip_path , unsigned int offset)
{
    gpio_init( chip_path , offset);
    return 1;
}

int read_IR_REC( void )
{   

    int value = 1;

    unsigned long long start_time = getTimeInMs();
    unsigned long long end_time = start_time;

    while( end_time - start_time  < 100 )
    {
        end_time = getTimeInMs();
        value = gpio_read();
        if( value == 0 )
        {
            return 1;
        }
    }

    return 0;


}