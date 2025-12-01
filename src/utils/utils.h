#ifndef UTILS_H
#define UTILS_H

int run_with_affinity(int cpu, void (*func)(void*), void* args);

#endif // UTILS_H