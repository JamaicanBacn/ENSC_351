#include "udp.h"
#include "beats.h"
#include <arpa/inet.h>
#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signalfd.h>
#include <unistd.h>

static bool command(char *packet, char *reply, size_t capacity)
{
    const char *ws = " \t\r\n";
    char *state;
    char *name = strtok_r(packet, ws, &state);
    char *arg = strtok_r(NULL, ws, &state);
    snprintf(reply, capacity, "error: invalid command or argument");
    if (!name || strtok_r(NULL, ws, &state)) return false;
    if (strcmp(name, "stop") == 0 && !arg) {
        snprintf(reply, capacity, "stopping");
        return true;
    }

    // Socket.IO can serialize a missing poll argument as null.
    bool query = !arg || strcmp(arg, "null") == 0 || strcmp(arg, "undefined") == 0;
    long value = 0;
    if (!query) {
        char *end;
        errno = 0;
        value = strtol(arg, &end, 10);
        if (errno || *end) return false;
    }

    if (strcmp(name, "mode") == 0) {
        if (!query && (value < NOTHING || value >= BEAT_COUNT)) return false;
        if (!query) changeBeat((BEATS)value);
        value = getBeat();
    } else if (strcmp(name, "volume") == 0) {
        if (!query && (value < 0 || value > AUDIOMIXER_MAX_VOLUME)) return false;
        if (!query) AudioMixer_setVolume(value);
        value = AudioMixer_getVolume();
    } else if (strcmp(name, "tempo") == 0) {
        if (!query && (value < 40 || value > 300)) return false;
        if (!query) changeBpm(value);
        value = getBpm();
    } else if (strcmp(name, "play") == 0) {
        static const int drum[] = {DRUM_BD_HARD, DRUM_CH, DRUM_SNARE_HARD, DRUM_BD_SOFT};
        if (query || value < 0 || value >= (long)(sizeof drum / sizeof drum[0])) return false;
        AudioMixer_queueSound(&sounds[drum[value]]);
    } else {
        return false;
    }
    snprintf(reply, capacity, "%ld", value);
    return false;
}

int udp_run(const sigset_t *signals)
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) return errno;
    struct sockaddr_in local = {.sin_family = AF_INET, .sin_port = htons(12345),
                                .sin_addr.s_addr = htonl(INADDR_LOOPBACK)};
    int signalFd = -1;
    int error = 0;
    if (bind(fd, (struct sockaddr *)&local, sizeof local) < 0 ||
        (signalFd = signalfd(-1, signals, SFD_CLOEXEC)) < 0) {
        error = errno;
    }
    struct pollfd inputs[] = {{fd, POLLIN, 0}, {signalFd, POLLIN, 0}};
    while (!error) {
        if (poll(inputs, 2, -1) < 0) {
            if (errno == EINTR) continue;
            error = errno;
            break;
        }
        if (inputs[1].revents & POLLIN) break;
        if (!(inputs[0].revents & POLLIN)) {
            error = EIO;
            break;
        }
        struct sockaddr_in peer;
        socklen_t len = sizeof peer;
        char packet[128], reply[64] = "error: invalid packet";
        ssize_t n = recvfrom(fd, packet, sizeof packet, MSG_TRUNC,
                             (struct sockaddr *)&peer, &len);
        if (n < 0) {
            if (errno == EINTR) continue;
            error = errno;
            break;
        }
        bool stop = false;
        if ((size_t)n < sizeof packet && !memchr(packet, '\0', n)) {
            packet[n] = '\0';
            stop = command(packet, reply, sizeof reply);
        }
        if (sendto(fd, reply, strlen(reply), 0, (struct sockaddr *)&peer, len) < 0)
            perror("UDP reply");
        if (stop) break;
    }
    if (signalFd >= 0) close(signalFd);
    close(fd);
    return error;
}
