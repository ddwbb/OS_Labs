#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/types.h>

#define COUNTER_MAX 9
#define MEMORY_SIZE 11

int data_counter = 0;
char * buffer;

pthread_rwlock_t rwlock;

void init();
void * produce();
void * consume();

int main(int argc, char * argv[]) {
    pthread_t producer;
    pthread_t consumer;

    init();

    pthread_rwlock_init(&rwlock, NULL);

    pthread_create(&producer, NULL, produce, NULL);
    pthread_create(&consumer, NULL, consume, NULL);

    sleep(15);

    pthread_cancel(producer);
    pthread_cancel(consumer);

    pthread_rwlock_destroy(&rwlock);
}

void * consume() {
    while (1) {
        if (data_counter >= COUNTER_MAX) {
            pthread_rwlock_rdlock(&rwlock);

            for (; data_counter; data_counter--)
                buffer[data_counter] = ' ';

            printf("Nyam! Now it's empty!\n");
            pthread_rwlock_unlock(&rwlock);
        }
    }
}

void * produce() {
    while(1) {
        if (data_counter < COUNTER_MAX) {
            pthread_rwlock_wrlock(&rwlock);
            usleep(500000);
            buffer[data_counter] = 'T';
            data_counter++;
            printf("Data: %d - %s\n", data_counter, buffer);
            if (data_counter >= COUNTER_MAX) {
                printf("Storage is full\n");
            }
            pthread_rwlock_unlock(&rwlock);
        }
    }
}

void init() {
    key_t key = 3792;
    int shm_id;

    if ((shm_id = shmget(key, MEMORY_SIZE, IPC_CREAT | 0666)) == -1) {
        perror("Shared memory get");
        exit(EXIT_FAILURE);
    }

    if ((buffer = shmat(shm_id, buffer, 0)) == (char *) -1) {
        perror("Shmat");
        exit(EXIT_FAILURE);
    }
}
