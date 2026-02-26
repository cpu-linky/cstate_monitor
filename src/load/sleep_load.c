#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RANDOM_MIN_MS 200
#define RANDOM_MAX_MS 2000

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <milliseconds|-1 for random> <count>\n", argv[0]);
        return 1;
    }

    int duration = atoi(argv[1]);
    int count    = atoi(argv[2]);

    if ((duration < 0 && duration != -1) || count < 0) {
        fprintf(stderr, "duration must be >= 0 or -1 (random), count must be >= 0.\n");
        return 1;
    }

    if (duration == -1)
        srand((unsigned int)time(NULL));

    for (int i = 0; i < count; ++i) {
        int ms = (duration == -1)
            ? RANDOM_MIN_MS + rand() % (RANDOM_MAX_MS - RANDOM_MIN_MS + 1)
            : duration;

        struct timespec ts = {
            .tv_sec  = ms / 1000,
            .tv_nsec = (ms % 1000) * 1000000L
        };
        nanosleep(&ts, NULL);
        system("echo IM_STILL_STANDING > /dev/null");
    }

    return 0;
}