#include "config.h"

void write_time(char *);

int main (int argc, char * argv[]) {
    char * buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));
    mkfifo(FIFO, 0777);

    int fd = open(FIFO, O_WRONLY);
    if (fd == -1) {
        printf("Something wrong with open fifo\n");
        exit(EXIT_FAILURE);
    }
    write_time(buffer);
    write(fd, buffer, BUFFER_SIZE);
    close(fd);
}

void write_time(char * str) {
    time_t timer = time(0);
    struct tm timeval = *localtime(&timer);
    sprintf(str, "[%.2d:%.2d:%.2d]\n", timeval.tm_hour, timeval.tm_min, timeval.tm_sec);
}
