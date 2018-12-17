#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/types.h>

#define COUNTER_MAX 10
#define MEMORY_SIZE 11

int data_counter = 0;
char * buffer;

pthread_mutex_t mutex;
pthread_cond_t condition;

void * produce();
void * consume();
void init();

int main(int argc, char * argv[]) {

    pthread_t producer;
    pthread_t consumer;

    init();

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);

    pthread_create(&producer, NULL, produce, NULL);
    pthread_create(&consumer, NULL, consume, NULL);

    sleep(50);

    pthread_cancel(producer);
    pthread_cancel(consumer);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);
}

void * consume() {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (data_counter < COUNTER_MAX) {
            pthread_cond_wait(&condition, &mutex);
        }
        while (data_counter) {
            buffer[data_counter] = ' ';
            data_counter--;
            printf("Nyam! %s\n", buffer);
        }
        pthread_mutex_unlock(&mutex);
    }
}

void * produce() {
    while(1) {
        pthread_mutex_lock(&mutex);
        usleep(500000);
        buffer[data_counter] = 'T';
        data_counter++;
        if (data_counter == 1)
            printf("Start producing:\n");
        printf("Data: %d - %s\n", data_counter, buffer);
        if (data_counter > COUNTER_MAX) {
            printf("Storage is full\n");
            pthread_cond_broadcast(&condition);
            usleep(500000);
        }
        pthread_mutex_unlock(&mutex);
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
