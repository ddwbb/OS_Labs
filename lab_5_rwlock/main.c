#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#define COUNTER_MAX 10

int data_counter = 0;

pthread_rwlock_t rwlock;

void * produce();
void * consume();

int main(int argc, char * argv[]) {
    pthread_t producer;
    pthread_t consumer;

    rwlock = PTHREAD_RWLOCK_INITIALIZER;

    pthread_create(&consumer, NULL, consume, NULL);
    pthread_create(&producer, NULL, produce, NULL);

    sleep(15);

    pthread_cancel(producer);
    pthread_cancel(consumer);

    pthread_rwlock_destroy(&rwlock);
}

void * produce() {
    while (1) {
        if (data_counter >= COUNTER_MAX) {
            pthread_rwlock_rdlock(&rwlock);
            data_counter = 0;
            printf("Nyam! Now it's empty!\n");
            pthread_rwlock_unlock(&rwlock);
        }
    }
}

void * consume() {
    while(1) {
        if (data_counter <= COUNTER_MAX) {
            pthread_rwlock_wrlock(&rwlock);
            usleep(500000);
            data_counter++;
            printf("Data: %d\n", data_counter);
            if (data_counter > COUNTER_MAX) {
                printf("Storage is full\n");
            }
            pthread_rwlock_unlock(&rwlock);
        }
    }
}