#include "arch.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int binary_exists(char *bin_name) {
    char which_cmd[MAX_STR_LEN];
    char **cmd_output;
    int binary_exists;

    /* 2>&1 redirects stderr to stdout so that we catch errors */
    sprintf(which_cmd, "which %s 2>&1", bin_name);
    cmd_output = read_command_line_output(which_cmd);
    binary_exists = strstr(*cmd_output, "which: no") ? 0 : 1;
    free_command_line_output(cmd_output);
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
    char clone_cmd[MAX_STR_LEN];
    char make_cmd[MAX_STR_LEN];

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
