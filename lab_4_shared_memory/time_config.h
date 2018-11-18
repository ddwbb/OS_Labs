#ifndef _TIME_CONFIG_

#include <time.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define FILE_KEY "key"
#define MEMORY_SIZE 10

key_t get_key();

#endif
