#include <fcntl.h>
#include "time_config.h"

const char * time_wrap;

key_t get_key();

int main(int argc, char * argv[]) {
    key_t key = get_key();
    int shm_id;

    if ((shm_id = shmget(key, MEMORY_SIZE, 0666)) == -1) {
        perror("Shared memory get");
        exit(EXIT_FAILURE);
    }

    if ((time_wrap = shmat(shm_id, time_wrap, 0)) == (char *) -1) {
        perror("Shared memory attention");
        exit(EXIT_FAILURE);
    }

    while(1) {
        printf("%s\n", time_wrap);
        sleep(1);
    }
}


