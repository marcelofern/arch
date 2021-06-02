#include "cl_utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void dotfiles(void) {
    struct cl_it cl_it[1];
    char *git_status_cmd = "cd ~ && git status 2>&1";

    cl_open(cl_it, git_status_cmd);
    // only reads the first line from the iterator.
    cl_next(cl_it);
    char *missing_repo = strstr(cl_it->line, "fatal: not a git repository");
    cl_close(cl_it);

    if (missing_repo) {
        system("cd ~ && rm -rf dotfiles");
        system("cd ~ && git clone https://github.com/marcelofern/dotfiles.git");
        system("cd ~ && cp -rf dotfiles/.[!.]* . && rm -rf dotfiles");
        system("cd && git remote set-url origin git@github.com:marcelofern/dotfiles.git");
        system("cd ~ && git checkout .");
    }
    else {
        system("cd ~ && git pull origin main");
    }
    system("nitrogen --save --set-auto ~/Wallpapers/mountain-png.png");
}
