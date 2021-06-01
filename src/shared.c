#include "cl_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int binary_exists(char *bin_name) {
    struct cl_it cl_it[1];
    char *cmd = NULL;

    /* 2>&1 redirects stderr to stdout so that we catch errors */
    sprintf(cmd, "which %s 2>&1", bin_name);

    cl_open(cl_it, cmd);
    /* only grabs the first line from the iterator */
    cl_next(cl_it);
    int binary_exists = strstr(cl_it->line, "which: no") ? 0 : 1;
    cl_close(cl_it);
    return binary_exists;
}

void update_system_time(void)
{
    system("timedatectl set-timezone \"NZ\"");
    system("sudo ntpd -qg");
    system("sudo hwclock --systohc");
}

void install_aur_package(char *package_name)
{
    char *clone_cmd = NULL;
    char *make_cmd = NULL;

    sprintf(
        clone_cmd,
        "git clone https://aur.archlinux.org/%s.git",
        package_name
    );
    system(clone_cmd);

    sprintf(
        make_cmd,
        "cd %s && yes | makepkg -sirc && cd .. && rm -rf %s",
        package_name, package_name
    );
    system(make_cmd);
}
