#define _POSIX_C_SOURCE 2 // to allow popen and pclose.

#include "arch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void dotfiles(void) {
    char git_status_result[MAX_STR_LEN];
    FILE *fp;

    fp = popen("cd ~ && git status 2>&1", "r");
    while (fgets(git_status_result, sizeof(git_status_result), fp) != NULL) {
        ;
    }
    pclose(fp);

    printf("%s", git_status_result);
    if (strstr(git_status_result, "fatal: not a git repository")) {
        system("cd ~ && rm -rf dotfiles");
        system("cd ~ && git clone https://github.com/marcelofern/dotfiles.git");
        system("cd ~ && cp -rf dotfiles/.[!.]* . && rm -rf dotfiles");
        system("cd && git remote set-url origin git@github.com:marcelofern/dotfiles.git");
        system("cd ~ && git checkout .");
    }
    else {
        system("cd ~ && git pull origin main");
    }
}
