#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFFER_SIZE 80

void write_time(char *);

int main(int argc, char * argv[]) {
    int fd[2];
    pid_t pid;
    char * buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));

    pipe(fd);

    if ((pid = fork()) == 0) {
        close(fd[0]);
        for (int i = 0; i < 3; i++) {
            write_time(buffer);
            write(fd[1], buffer, BUFFER_SIZE);
            sleep(1);
        }
        close(fd[1]);
    } else {
        close(fd[1]);
        while (read(fd[0], buffer, BUFFER_SIZE))
            printf("Receive from child process: %s\n", buffer);
        close(fd[0]);
    }
}

void write_time(char * str) {
    time_t timer = time(0);
    struct tm timeval = *localtime(&timer);

    sprintf(str, "[%.2d:%.2d:%.2d]\n", timeval.tm_hour, timeval.tm_min, timeval.tm_sec);
}
