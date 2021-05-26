#include "arch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void dotfiles(void) {
    char **git_status_output;
    char *git_status_cmd = "cd ~ && git status 2>&1";

    git_status_output = read_command_line_output(git_status_cmd);
    if (strstr(*git_status_output, "fatal: not a git repository")) {
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
