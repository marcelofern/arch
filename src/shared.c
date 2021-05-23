#define _POSIX_C_SOURCE 2 // to allow popen and pclose.

#include "arch.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int binary_exists(char *bin_name) {
    char search_result[MAX_STR_LEN];
    char which_cmd[MAX_STR_LEN];
    FILE *fp;

    /* 2>&1 redirects stderr to stdout so that we catch errors */
    sprintf(which_cmd, "which %s 2>&1", bin_name);
    fp = popen(which_cmd, "r");
    while (fgets(search_result, sizeof(search_result), fp) != NULL) {
        ;
    }
    pclose(fp);

    if (strstr(search_result, "which: no"))
        return 0;
    else
        return 1;
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
