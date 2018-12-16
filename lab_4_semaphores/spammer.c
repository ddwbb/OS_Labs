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

void lock_semaphore();
void unlock_semaphore();
void open_semaphore();
void create_semaphore();
void init_shared_memory();
void spam();

int main(int argc, char * argv[]) {
    pid_t pid;
    init_shared_memory();
    create_semaphore();
    if (argc == 2) {
        if ((pid = fork()) == 0) {
            usleep(500000);
            execv("./connector", argv);
        } else {
            spam();
            kill(pid, SIGTERM);
            unlock_semaphore();
        }
    } else {
        spam();
    }
}

void create_semaphore() {
    key_t key = 3972;

    if ((sem_id = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
        if (errno == EEXIST) {
            open_semaphore();
        } else {
            perror("Semaphore creating");
            exit(0);
        }
    } else {
        printf("Semaphore created\n");
    }
    struct sembuf init = {0, 1, SEM_UNDO};
    semop(sem_id, &init, 1);
}

void open_semaphore() {
    key_t key = 3972;

    if ((sem_id = semget(key, 1, 0666)) == -1) {
        perror("Semaphore opening");
        exit(0);
    }
}

void init_shared_memory() {
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

void spam() {
    struct tm timeval;
    for(int i = 0; i < 10; i++) {
        lock_semaphore();
        time_t timer = time(0);
        timeval = *localtime(&timer);
        sprintf(buffer, "%.2d:%.2d:%.2d", timeval.tm_hour, timeval.tm_min, timeval.tm_sec);
        unlock_semaphore();
        sleep(1);
    }
}