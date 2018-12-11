#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FIFO "./fifo_file"
#define BUFFER_SIZE 80

char * buffer;
int fd;

void * sender_thread();
void write_time(char *);

int main (int argc, char * argv[]) {
    buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));
    mkfifo(FIFO, 0777);
    pthread_t thread;

    fd = open(FIFO, O_WRONLY);
    if (fd == -1) {
        printf("Something wrong with open fifo\n");
        exit(EXIT_FAILURE);
    }

    pthread_create(&thread, NULL, sender_thread, NULL);

    while(1) {
        sleep(1);
    }
}

void write_time(char * str) {
    time_t timer = time(0);
    struct tm timeval = *localtime(&timer);
    sprintf(str, "[%.2d:%.2d:%.2d]\n", timeval.tm_hour, timeval.tm_min, timeval.tm_sec);
}

void * sender_thread() {
    while(1) {
        write_time(buffer);
        write(fd, buffer, BUFFER_SIZE);
        sleep(1);
    }
}