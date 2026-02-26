#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <milliseconds> <count>\n", argv[0]);
        return 1;
    }

    int milliseconds = atoi(argv[1]);
    int count = atoi(argv[2]);

    if (milliseconds < 0 || count < 0) {
        fprintf(stderr, "Both values must be non-negative.\n");
        return 1;
    }

    struct timespec ts = {
        .tv_sec  = milliseconds / 1000,
        .tv_nsec = (milliseconds % 1000) * 1000000L
    };

    for (int i = 0; i < count; ++i) {
        nanosleep(&ts, NULL);
        system("echo IM_STILL_STANDING > /dev/null");
    }

    return 0;
}