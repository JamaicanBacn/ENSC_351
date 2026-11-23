#ifndef UDP_H
#define UDP_H
#include <signal.h>

// Serve the local Node relay until stop or a blocked termination signal arrives.
// Returns zero on orderly shutdown, otherwise an errno value.
int udp_run(const sigset_t *signals);
#endif
