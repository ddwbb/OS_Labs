#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

int sem_id;
char * buffer;

void open_semaphore();
void lock_semaphore();
void unlock_semaphore();
void init_shared_memory();
void read_data();

int main(int argc, char * argv[]) {
    init_shared_memory();
    open_semaphore();
    read_data();
}

void init_shared_memory() {
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

void open_semaphore() {
    key_t key = 3972;

    if ((sem_id = semget(key, 1, 0666)) == -1) {
        perror("Semaphore opening");
        exit(0);
    }
}

void lock_semaphore() {
    struct sembuf lock = { 0, -1, SEM_UNDO };

    if (semop(sem_id, &lock, 1) == -1) {
        perror("Lock failed");
        exit(0);
    }
    printf("Semaphore locked\n");
}

void unlock_semaphore() {
    struct sembuf unlock = { 0, 1, SEM_UNDO };

    if (semop(sem_id, &unlock, 1) == -1) {
        perror("Unlock failed");
        exit(0);
    }
    printf("Semaphore unlocked\n");
}

void read_data() {
    while(1) {
        lock_semaphore();
        printf("%s\n", buffer);
        unlock_semaphore();
        sleep(1);
    }
}

