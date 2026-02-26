#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "src/env/env.h"
#include "src/utils/utils.h"

void purge_cache() {
    system("sync");
    system("echo 3 | sudo tee /proc/sys/vm/drop_caches > /dev/null");
}

static void run_load(const char *name, char *const *command, const char *log_path,
                     int n_cycles, int do_purge_cache) {
                        
    printf("[INFO] %s test begins : %d cycles\n", name, n_cycles);
    for (int i = 0; i < n_cycles; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            int fd = open(log_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1) {
                perror("[ERROR] Failed to open log file");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);

            if (do_purge_cache)
                purge_cache();

            execvp(command[0], command);
            perror("[ERROR] execvp failed");
            exit(1);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
            printf("[INFO] %s load cycle no %d done! (Exit code : %d)\n", name, i, WEXITSTATUS(status));
        } else {
            perror("[ERROR] Failed to fork");
        }
    }
    printf("[SUCCESS] %s : all %d cycles are done\n", name, n_cycles);
}

int main() {
    load_env_file("config.env");

    char *cpu_n_pi         = getenv("CPU_N_PI");
    int   n_cpu            = atoi(getenv("N_CPU"));
    char *cpu_log_path     = getenv("CPU_LOG_PATH");

    char *mem_n_jumps      = getenv("MEMORY_N_JUMPS");
    int   n_memory         = atoi(getenv("N_MEMORY"));
    char *memory_log_path  = getenv("MEMORY_LOG_PATH");

    int   io_purge_cache   = atoi(getenv("IO_PURGE_CACHE"));
    char *io_n_dumps       = getenv("IO_N_DUMPS");
    char *io_random_offset = getenv("IO_RANDOM_OFFSET");
    char *io_size_dump     = getenv("IO_SIZE_DUMP");
    int   n_io             = atoi(getenv("N_IO"));
    char *io_log_path      = getenv("IO_LOG_PATH");

    char *cpu_target_load      = getenv("CPU_TARGET_LOAD");
    int   cpu_target_turbostat = atoi(getenv("CPU_TARGET_TURBOSTAT"));

    char *command_cpu[] = {
        "turbostat", "--quiet", "--interval", "1", "--cpu", cpu_target_load,
        "taskset", "-c", cpu_target_load,
        "bin/cpu_load", cpu_n_pi, NULL
    };

    char *command_mem[] = {
        "turbostat", "--quiet", "--interval", "1", "--cpu", cpu_target_load,
        "taskset", "-c", cpu_target_load,
        "bin/memory_load", mem_n_jumps, NULL
    };

    char *command_io[] = {
        "turbostat", "--quiet", "--interval", "1", "--cpu", cpu_target_load,
        "taskset", "-c", cpu_target_load,
        "bin/io_load", io_n_dumps, io_size_dump, io_random_offset, NULL
    };

    set_affinity(cpu_target_turbostat);

    run_load("CPU",    command_cpu, cpu_log_path,    n_cpu,    0);
    run_load("Memory", command_mem, memory_log_path, n_memory, 0);
    run_load("I/O",    command_io,  io_log_path,     n_io,     io_purge_cache);

    return 0;
}