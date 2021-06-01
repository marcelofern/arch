#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

/* structure to iterate over a command line output.*/
struct cl_it {
    char *line;
    size_t cap;
    ssize_t len;
    char *err;
    FILE *fp;
};

char *cl_next(struct cl_it *cl_it);
int cl_close(struct cl_it *cl_it);
int cl_open(struct cl_it *cl_it, const char *cmd);
