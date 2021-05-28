#include "arch.h"

#define _POSIX_C_SOURCE 2 // to allow popen and pclose.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Reads the result of a command line call into an array of strings ("output").
 * Returns a pointer to an array containing these strings.
 * Needs to be freed by `free_command_line_output()` after used.
 *
 * USAGE:
 *
 *  output = read_comand_line_output(cmd);
 *  for(int i = 0; cmd_output[i] != NULL; i++)
 *    printf("%s\n", *cmd_output);
 *  free_command_line_output(output);
 */
char **read_command_line_output(char *cmd)
{
    FILE *fp;
    char buffer[MAX_STR_LEN];  // buffer that holds a line.
    static char *output[MAX_NUM_LINES];
    int lines = 0;

    fp = popen(cmd, "r");
    while (fgets(buffer, sizeof(buffer), fp) && lines < MAX_NUM_LINES) {
        buffer[strcspn(buffer, "\n")] = 0;  // trims out the \n character.
        output[lines] = malloc(sizeof(buffer));
        // buffer's value will cease to exist in the next interaction, so we
        // need to copy its value to the output via memcpy or strcpy
        memcpy(output[lines++], buffer, MAX_STR_LEN);
    }
    output[lines+1] = NULL;  // end of the array
    pclose(fp);
    return output;
}

void free_command_line_output(char **output)
{
    for (int i = 0; output[i] != NULL; i++) {
        free(output[i]);
    }
}
