#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <seconds> <count>\n", argv[0]);
        return 1;
    }

    int seconds = atoi(argv[1]);
    int count = atoi(argv[2]);

    if (seconds < 0 || count < 0) {
        fprintf(stderr, "Both values must be non-negative.\n");
        return 1;
    }

    for (int i = 0; i < count; ++i) {
        sleep((unsigned int)seconds);
    }

    return 0;
}