// Created by ddwbb on 20.09.2018.

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void child_process ();
void parent_process ();
void print_info (pid_t, char *);

int main (int argc, char * argv[]) {
    pid_t pid;

    if ((pid = fork()) == 0) {
        child_process();
    } else {
        parent_process();
    }

    exit(EXIT_SUCCESS);
}

void child_process () {
    pid_t pid = getpid();
    print_info(pid, "Child process has arrived");
    sleep(3);
    print_info(pid, "Now I'm going to die... Save me");
    exit(EXIT_SUCCESS);
}

void parent_process () {
    pid_t pid = getpid();
    int child_exit_status;
    print_info(pid, "Main work...");
    wait(&child_exit_status);
    sleep(1);
    print_info(pid, "End work");
}

void print_info (pid_t pid, char * str) {
    time_t t = time(0);
    struct tm * timer = localtime(&t);
    printf("[%d:%d:%d][PID: %d]: %s\n", timer->tm_hour, timer->tm_min, timer->tm_sec, pid, str);
    fflush(stdout);
}