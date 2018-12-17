#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#define FILE_SIZE 314572800
#define BLOCK_SIZE 1024

struct program {
    char * base;                                    //Где создать файл
    char * path;                                    //Куда переместить
} args;

void init(int, char * []);
void generate();
void fill_buffer(char *);
char * get_path(char *, char *);
void move();
void print_action_time(const char *);

int main (int argc, char * argv[]) {
    pid_t pid;
    time_t _start = time(0);
    init(argc, argv);
    if ((pid = fork())) {
        generate();
        int status;
        wait(&status);
        time_t _end = time(0);
        printf("Total work time: %lld\n", (long long)(_end - _start));
    } else {
        sleep(1);
        move();
    }
}

void init (int argc, char * argv []) {
    switch (argc) {
        case 1:
            args.base = "./";
            args.path = "./";
            break;
        case 2:
            args.base = "./";
            args.path = argv[1];
            break;
        case 3:
            args.base = argv[1];
            args.path = argv[2];
            break;
        default:
            printf("Too much args! \n");
            exit(EXIT_FAILURE);
    }
    srand(time(0));
    print_action_time("Start");
}

void generate () {

    int fd;
    char * buffer = (char *)calloc(BLOCK_SIZE, sizeof(char));

    if ((fd = open(get_path(args.base, "from"), O_RDWR | O_CREAT, S_IWRITE | S_IREAD)) == -1) {
        perror("Cannot open file");
        exit(EXIT_FAILURE);
    }

    fill_buffer(buffer);
    for (int i = 0; i < FILE_SIZE / BLOCK_SIZE; i++) {
        write(fd, buffer, BLOCK_SIZE);
    }

    close(fd);
    print_action_time("File was generated");
}

void fill_buffer (char * buffer) {
    for (int i = 0; i < BLOCK_SIZE; i++)
        buffer[i] = rand() % 26 + 97;           //Rand char a-z
}

void move () {

    int fd_in, fd_out;
    char * buffer = (char *)calloc(BLOCK_SIZE, sizeof(char));

    if ((fd_in = open(get_path(args.base, "from"), O_RDWR)) == -1) {
        perror("Cannot open input file");
        exit(EXIT_FAILURE);
    }

    if ((fd_out = open(get_path(args.path, "to"), O_RDWR | O_CREAT, S_IWRITE | S_IREAD)) == -1) {
        perror("Cannot open output file");
        exit(EXIT_FAILURE);
    }

    int data_count;

    for (int i = 0; i < FILE_SIZE / BLOCK_SIZE; i++) {
        data_count = read(fd_in, buffer, BLOCK_SIZE);
        if (!data_count) break;
        write(fd_out, buffer, BLOCK_SIZE);
    }

    close(fd_in);
    close(fd_out);

    print_action_time("File moved");
}

char * get_path (char * arg, char * name) {
    char * path = (char *)calloc(strlen(arg) + 10, sizeof(char));
    strcpy(path, arg);
    strcat(path, name);
    return path;
}

void print_action_time (const char * str) {
    time_t timer = time(0);
    struct tm timeval = *localtime(&timer);

    printf("[%.2d:%.2d:%.2d]: %s\n", timeval.tm_hour, timeval.tm_min, timeval.tm_sec, str);
}