#include "time_config.h"

key_t get_key() {
    FILE * fp;
    int id = 0;
    key_t key = 0;

    if ((fp = fopen(FILE_KEY, "r")) == NULL) {
        perror("Open key file");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d", &id);
    key = ftok("./key", id);
    return key;
}
