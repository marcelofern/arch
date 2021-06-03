/** @file 
 Command line (cl) utility functions.
 USAGE:
 ```
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
 ```
 */
#ifndef CL_UTILS_H
#define CL_UTILS_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

/**
 * Structure to iterate over a command line output.
*/
struct cl_it {
    /**
     * Line containing the text read from the command line output.
     */
    char *line;
    /**
     * Size stored in memory for the `line` buffer. This is also the second
     * argument (r) of the `getline` function.
     */
    size_t cap;
    /**
     * Size of `line` itself in bytes. Can be directly translated to the number
     * of characters in the line.
     */
    ssize_t len;
    /**
     * The string containing the error, if any. Directly translated from
     * `strerror(errorno)`.
     */
    char *err;
    /**
     * Pointer to the file representing the command line output in the current
     * reading
     */
    FILE *fp;
};

char *cl_next(struct cl_it *cl_it);
int cl_close(struct cl_it *cl_it);
int cl_open(struct cl_it *cl_it, const char *cmd);
#endif // ifndef CL_UTILS_H
