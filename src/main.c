/*
 * Marcelo's arch linux installation and updating tools.
 * ----------------------------------------------------
 *
 *  Dependencies:
 *    1 - `base-devel` for building this package.
 *    2 - `git` for cloning this repo.
 *
 *  Installation:
 *    1 - TBD.
 *
 *  Upgrading:
 *    1 - TBD.
 */
#include "arch.h"
#include <stdlib.h>
#include <argp.h>

const char *argp_program_version = "v1.0";
const char *argp_program_bug_address = "<marceelofernandes@gmail.com>";
static char doc[] = "";
static char args_doc[] = "";

static struct argp_option options[] = {
    {"install", 'i', 0, 0, "Install environment dependencies from scratch.", 0},
    {"update", 'u', 0, 0, "Update environment dependencies.", 0},
    {"dotfiles", 'd', 0, 0, "Install or sync dotfiles.", 0},
    {"sync", 's', 0, 0, "Sync this script with its latest code.", 0},
    {0},
};

struct arguments
{
    int install;
    int update;
    int dotfiles;
    int sync;
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    /*
    * Get the input argument from argp_parse, which we
    * know is a pointer to our arguments structure.
    */
    struct arguments *arguments = state->input;

    switch (key) {
        case 'i':
            arguments->install = 1;
            break;
        case 'u':
            arguments->update = 1;
            break;
        case 'd':
            arguments->dotfiles = 1;
            break;
        case 's':
            arguments->sync = 1;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
static struct argp argp = {options, parse_opt, args_doc, doc};
#pragma GCC diagnostic pop

int main(int argc, char **argv) {
    struct arguments arguments;
    int count;

    /* Default values. */
    arguments.install = 0;
    arguments.update = 0;
    arguments.dotfiles = 0;
    arguments.sync = 0;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    count = (
        arguments.install + arguments.update
        + arguments.dotfiles + arguments.sync
    );
    if (count != 1) {
        fprintf(stderr, "This command can only take exactly one flag.\n");
        exit(1);
    }
    if (arguments.install)
        install();
    else if (arguments.update)
        update();
    else if (arguments.dotfiles)
        dotfiles();
    else if (arguments.sync)
        sync();
    exit(0);
}
