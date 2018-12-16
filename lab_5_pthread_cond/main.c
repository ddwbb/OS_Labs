#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#define COUNTER_MAX 10

int data_counter = 0;

pthread_mutex_t mutex;
pthread_cond_t condition;

void * produce();
void * consume();

int main(int argc, char * argv[]) {
    pthread_t producer;
    pthread_t consumer;

    condition = PTHREAD_COND_INITIALIZER;

    pthread_create(&consumer, NULL, consume, NULL);
    pthread_create(&producer, NULL, produce, NULL);

    sleep(15);

    pthread_cancel(producer);
    pthread_cancel(consumer);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);
}

void * produce() {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (data_counter < COUNTER_MAX) {
            pthread_cond_wait(&condition, &mutex);
        }
        data_counter = 0;
        printf("Nyam!\n");
        pthread_mutex_unlock(&mutex);
    }
}

void * consume() {
    while(1) {
        pthread_mutex_lock(&mutex);
        usleep(500000);
        data_counter++;
        printf("Data: %d\n", data_counter);
        if (data_counter >= COUNTER_MAX) {
            printf("Storage is full\n");
            pthread_cond_broadcast(&condition);
        }
        pthread_mutex_unlock(&mutex);
    }
}