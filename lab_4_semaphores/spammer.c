#include <time.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

char * buffer;

void init();
void spam();

int main(int argc, char * argv[]) {
    init();
    spam();
}

void init() {
    key_t key = 3972;
    int shm_id;

    if ((shm_id = shmget(key, 1024, IPC_CREAT | 0666)) == -1) {
        perror("Shared memory get");
        exit(EXIT_FAILURE);
    }

    if ((buffer = shmat(shm_id, buffer, 0)) == (char *) -1) {
        perror("Shmat");
        exit(EXIT_FAILURE);
    }
}

void spam() {
    struct tm timeval;
    while(1) {
        time_t timer = time(0);
        timeval = *localtime(&timer);
        sprintf(buffer, "%.2d:%.2d:%.2d", timeval.tm_hour, timeval.tm_min, timeval.tm_sec);
        sleep(1);
    }
}