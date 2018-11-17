#include <time.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define FILE_KEY "key"
//
#define MEMORY_SIZE 27

const char * time_wrap;

void spam();
void init();
int get_key();

int main (int argc, char * argv[]) {
    init();
    spam();
}

void init() {
    key_t key = get_key();
    int shm_id;

    printf("Ket: %lld\n", key);

    if ((shm_id = shmget(key, MEMORY_SIZE, IPC_CREAT | 0666)) == -1) {
        perror("Shared memory get");
        exit(EXIT_FAILURE);
    }
}

void spam() {
    while(1) {
        time_t timer = time(0);
        struct tm timeval = *localtime(&timer);
    }
}

int get_key() {
    FILE * fp;
    key_t key = 0;

    if ((fp = fopen(FILE_KEY, "r")) == NULL) {
        perror("Open key file");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%lld", &key);
    return key;
}