#include "arch.h"
#include <stdio.h>
#include <stdlib.h>

static void update_from_yay();

void update(void)
{
    printf(YEL "updating hardware and system clock via ntpd..." RESET "\n");
    update_system_time();
    printf(GRN "hardware and system clock updated!" RESET "\n");

    printf(YEL "upgrading all packages..." RESET "\n");
    update_from_yay();
    printf(GRN "all packages updated!" RESET "\n");
}

static void update_from_yay(void) {
    if (!binary_exists("yay"))
        // print WARNING (installing yay)"
        install_aur_package("yay");
    system("yay -Syu --answerclean=None --answerdiff=None --answerclean=All");
}
