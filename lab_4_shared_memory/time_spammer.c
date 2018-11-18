#include "time_config.h"

char * time_wrap;

void spam();
void init();

int main (int argc, char * argv[]) {
    init();
    spam();
}

void init() {
    key_t key = get_key();
    int shm_id;

    if ((shm_id = shmget(key, MEMORY_SIZE, IPC_CREAT | 0666)) == -1) {
        perror("Shared memory get");
        exit(EXIT_FAILURE);
    }

    if ((time_wrap = shmat(shm_id, time_wrap, 0)) == (char *) -1) {
        perror("Shmat");
        exit(EXIT_FAILURE);
    }
}

void spam() {
    struct tm timeval;
    while(1) {
        time_t timer = time(0);
        timeval = *localtime(&timer);
        sprintf(time_wrap, "%.2d:%.2d:%.2d", timeval.tm_hour, timeval.tm_min, timeval.tm_sec);
        sleep(1);
    }
}
