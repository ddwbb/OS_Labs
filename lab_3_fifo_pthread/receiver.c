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

int fd;
char * time_wrap;

void init();
void * receive_thread();

int main(int argc, char * argv[]) {
    char input;
    pthread_t receiver;
    init();
    pthread_create(&receiver, NULL, receive_thread, NULL);
    scanf("%c", &input);
    if (input) {
        pthread_cancel(receiver);
        close(fd);
        exit(EXIT_SUCCESS);
        return 0;
    }
}

void init() {
    time_wrap = (char *)calloc(BUFFER_SIZE, sizeof(char));
    mkfifo(FIFO, 0777);

    fd = open(FIFO, O_RDONLY);
    if (fd == -1) {
        printf("Something wrong with open fifo\n");
        exit(EXIT_FAILURE);
    }
}

void * receive_thread() {
    while(1) {
        if (read(fd, time_wrap, BUFFER_SIZE) <= 0) {
            printf("Cannot read fifo\n");
            exit(0);
        }
        printf("Receive from other process: %s\n", time_wrap);
        sleep(1);
    }
}