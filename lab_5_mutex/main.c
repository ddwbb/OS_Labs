#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

pthread_mutex_t mutex;
char * buffer;

void * thread_send();
void * thread_receive();

int main() {
    buffer = (char *)calloc(40, sizeof(char));

    pthread_t sender;
    pthread_t receiver;

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex init");
        exit(0);
    }

    pthread_create(&sender, NULL, thread_send, NULL);
    usleep(500000);
    pthread_create(&receiver, NULL, thread_receive, NULL);

    sleep(11);
    pthread_cancel(sender);
    pthread_cancel(receiver);

    pthread_mutex_destroy(&mutex);
}

void * thread_send() {
    time_t timer;
    struct  tm timeval;
    while(1) {
        pthread_mutex_trylock(&mutex);
        timer = time(0);
        timeval = *localtime(&timer);
        sprintf(buffer, "%.2d:%.2d:%.2d", timeval.tm_hour, timeval.tm_min, timeval.tm_sec);
        sleep(1);
        pthread_mutex_unlock(&mutex);
    }
}

void * thread_receive() {
    while(1) {
        pthread_mutex_trylock(&mutex);
        printf("Time: %s\n", buffer);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}