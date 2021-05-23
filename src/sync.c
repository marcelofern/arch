#include <stdlib.h>

void sync(void) {
    system("rm -rf arch");
    system("cd /usr/local/bin && sudo mv arch arch.old");
    system("git clone https://github.com/marcelofern/arch.git");
    system("cd arch && make");
    system("rm -rf arch");
}
