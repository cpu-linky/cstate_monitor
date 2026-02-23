#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// dump a buffer of random data (pre made file) into a dump file
int dump_io(int n_mo){
    int fd_in, fd_out;
    size_t size = 1024*1024*n_mo;
    char *buffer = malloc(size); // Allocation sur le tas

    if (buffer == NULL) {
        perror("Error allocating memory");
        return -1;
    }

    fd_in = open("random.bin", O_RDONLY);
    if(fd_in < 0){
        perror("Error opening targeted file");
        free(buffer);
        return -1;
    }

    ssize_t result = read(fd_in, buffer, size);
    if (result != size) {
        perror("Reading error");
    }
    close(fd_in);

    fd_out = open("dump", O_WRONLY | O_CREAT | O_SYNC, 0644);
    if(fd_out < 0){
        perror("Error opening dump");
        free(buffer);
        return -1;
    }

    ssize_t written = write(fd_out, buffer, size);
    if (written != size) {
        perror("Writing error");
    }
    close(fd_out);
    free(buffer); // Ne pas oublier de libérer la mémoire

    // delete file
    if(unlink("dump") < 0){
        perror("Error deleting dump");
        return -1;
    }

    return 0;
}

void purge_cache(){
    system("sync");
    system("echo 3 | sudo tee /proc/sys/vm/drop_caches > /dev/null");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s <loop> <buff_size>\n", argv[0]);
        return 1;
    }

    int loop = atoi(argv[1]);
    int dump_size = atoi(argv[2]);
    for (int i = 0; i < loop; i++) {
        // purge_cache();
        if (dump_io(dump_size) != 0) {
            return 1;
        }
    }

    printf("#| Dumped %d * %dMB of random data to dump.bin\n", loop, dump_size);
    return 0;
}