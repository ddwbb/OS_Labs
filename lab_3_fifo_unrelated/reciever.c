#include "config.h"

int main (int argc, char * argv[]) {
    char * buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));
    mkfifo(FIFO, 0777);

    int fd = open(FIFO, O_RDONLY);
    read(fd, buffer, BUFFER_SIZE);
    printf("Receive from other process: %s\n", buffer);
    close(fd);
}