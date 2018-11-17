#include <io.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define FILE_SIZE 314572800                 //300 MB
#define BLOCK_SIZE 1024

struct program {
    char * base;
    char * path;
} args;

void init(int, char * []);
void generate();
void fill_buffer(char *);
char * get_path(char *);
void move();
void print_action_time(const char *);
void set_noblock(int, int);

int main (int argc, char * argv[]) {
    time_t _start = time(0);
    init(argc, argv);
    generate();
    move();
    time_t _end = time(0);
    printf("Total work time: %lld\n", (long long)(_end - _start));
}

void init (int argc, char * argv []) {
    switch (argc) {
        case 1:
            args.base = "./";
            args.path = "../";
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

void fill_buffer (char * buffer) {
    for (int i = 0; i < BLOCK_SIZE; i++)
        buffer[i] = rand() % 26 + 97;           //Rand char a-z
}

void print_action_time (const char * str) {
    time_t timer = time(0);
    struct tm timeval = *localtime(&timer);

    printf("[%.2d:%.2d:%.2d]: %s\n", timeval.tm_hour, timeval.tm_min, timeval.tm_sec, str);
}

char * get_path (char * arg) {
    char * path = (char *)calloc(strlen(arg) + 10, sizeof(char));
    strcpy(path, arg);
    strcat(path, "generated");
    return path;
}

void generate() {
    int fd;
    char * buffer = (char *)calloc(BLOCK_SIZE, sizeof(char));

    //print_action_time(get_path(args.base));

    if ((fd = open(get_path(args.base), O_RDWR | O_CREAT, S_IWRITE | S_IREAD)) == -1) {
        perror("Cannot open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < FILE_SIZE / BLOCK_SIZE; i++) {

        fill_buffer(buffer);
        write(fd, buffer, BLOCK_SIZE);
    }

    close(fd);
    print_action_time("File was generated");
}

void move() {
    fd_set read_descriptor;
    fd_set write_descriptor;

    struct timeval tv;

    int value;
    int fd_in;
    int fd_out;

    char * buffer = (char *)calloc(BLOCK_SIZE, sizeof(char));

    if ((fd_in = open(get_path(args.base), O_RDONLY | O_NONBLOCK)) == -1) {
        perror("Cannot open input file");
        exit(EXIT_FAILURE);
    }

    if ((fd_out = open(get_path(args.path), O_WRONLY | O_CREAT | O_NONBLOCK)) == -1) {
        perror("Cannot open output file");
        exit(EXIT_FAILURE);
    }

    set_noblock(fd_in, 0);
    set_noblock(fd_out, 0);

    FD_ZERO(&read_descriptor);
    FD_ZERO(&write_descriptor);

    FD_SET(fd_in, &read_descriptor);
    FD_SET(fd_out, &write_descriptor);

    int max_fd = (fd_in > fd_out) ? fd_in : fd_out;
    int bytes = -1;

    tv.tv_sec = 3;
    tv.tv_usec = 0;

    if ((value = select(max_fd, &read_descriptor, &write_descriptor, NULL, &tv)) == -1) {
        perror("Select()");
    } else if (value) {
        for (;;){
            if (FD_ISSET(fd_in, &read_descriptor)) {
                bytes = read(fd_in, buffer, BLOCK_SIZE);
                if (bytes > 0) {
                    write(fd_out, buffer, BLOCK_SIZE);
                } else {
                    break;
                }
            }
        }
    } else {
        print_action_time("No data within 3 sec");
    }

    close(fd_in);
    close(fd_out);

    print_action_time("File moved");
}

void set_noblock(int fd, int blocking) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) exit(EXIT_FAILURE);
    if (blocking) flags &= ~O_NONBLOCK;
    else flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
}

