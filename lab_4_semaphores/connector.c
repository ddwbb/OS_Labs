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
void read_data();

int main(int argc, char * argv[]) {
    init();
    read_data();
}

void init() {
    key_t key = 3972;
    int shm_id;

    if ((shm_id = shmget(key, 1024, 0666)) == -1) {
        perror("Shared memory get");
        exit(EXIT_FAILURE);
    }

    if ((buffer = shmat(shm_id, buffer, 0)) == (char *) -1) {
        perror("Shared memory attention");
        exit(EXIT_FAILURE);
    }
}

void read_data() {
    while(1) {
        printf("%s\n", buffer);
    }
}

