/*
 * Command line (cl) utility functions.
 *
 * USAGE:
    int main(void)
    {
        struct cl_it cl_it[1];
        if (cl_open(cl_it, "<your cl command>") == -1) {
            fprintf(stderr, "fatal: %s\n", cl_it->err);
            return 1;
        }

        int count = 0;
        while (cl_next(cl_it)) {
            printf("%d -> %s", ++count, cl_it->line);
        }

        int r = cl_close(cl_it);
        if (cl_it->err) {
            fprintf(stderr, "fatal: %s\n", cl_it->err);
            return 1;
        }
        if (r != 0) {
            fprintf(stderr, "fatal: subprocess exited with status %d\n", r);
            return 1;
        }
        return 0;
    }
 *
 */

#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* structure to iterate over a command line output.*/
struct cl_it {
    char *line;
    size_t cap;
    ssize_t len;
    char *err;
    FILE *fp;
};

int cl_open(struct cl_it *cl_it, const char *cmd)
{
    if (!(cl_it->fp = popen(cmd, "r"))) {
        cl_it->err = strerror(errno);
        return -1;
    }
    return 0;
}

char *cl_next(struct cl_it *cl_it)
{
    cl_it->len = getline(&cl_it->line, &cl_it->cap, cl_it->fp);
    if (cl_it->len == -1) {
        char *status = strerror(errno);
        // EOF returns -1 but it is actually a success.
        cl_it->err = strcmp(status, "Success") ? status : 0;
        cl_it->len = 0;
        return 0;
    }
    return cl_it->line;
}

int cl_close(struct cl_it *cl_it)
{
    int r = pclose(cl_it->fp);
    if (r == -1)
        cl_it->err = strerror(errno);
    free(cl_it->line);
    return r == -1 ? r : WEXITSTATUS(r);
}
