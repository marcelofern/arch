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
    {"install", 'i', 0, 0, "Install environment dependencies from scratch."},
    {"update", 'u', 0, 0, "Update environment dependencies."},
    {"dotfiles", 'd', 0, 0, "Install or sync dotfiles."},
    {0},
};

struct arguments
{
    int install;
    int update;
    int dotfiles;
};

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
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv) {
    struct arguments arguments;
    int count;

    /* Default values. */
    arguments.install = 0;
    arguments.update = 0;
    arguments.dotfiles = 0;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    count = arguments.install + arguments.update + arguments.dotfiles;
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
    exit(0);
}
