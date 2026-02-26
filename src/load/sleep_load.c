#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RANDOM_MIN_NS 1
#define RANDOM_MAX_NS 999999999

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <ms> <ns|-1 for random> <count> <log = 1|0> \n", argv[0]);
        return 1;
    }

    int duration_ms = atoi(argv[1]);
    int duration_ns = atoi(argv[2]);
    int count = atoi(argv[3]);
    int log = atoi(argv[4]);

    if ((duration_ms < 0 && duration_ms != -1) || count < 0) {
        fprintf(stderr, "duration must be >= 0 or -1 (random), count must be >= 0.\n");
        return 1;
    }

    if (duration_ns == -1)
        srand((unsigned int)time(NULL));

    for (int i = 0; i < count; ++i) {
        int ns = (duration_ns == -1)
            ? RANDOM_MIN_NS + (int)((double)rand() / RAND_MAX * (RANDOM_MAX_NS - RANDOM_MIN_NS))
            : duration_ns;

        struct timespec ts = {
            .tv_sec  = duration_ms / 1000,
            .tv_nsec = ns,
        };
        nanosleep(&ts, NULL);
        if (log == 1){
            printf("#| Slept for %d ms and %d ns\n", duration_ms, ns);
        }
    }

    return 0;
}