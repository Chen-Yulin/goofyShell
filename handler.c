#include "handler.h"

void signal_handler(int signo) {
    if (signo == 2) {
        if (jobNum == 0) {
            printf("\nmumsh $ ");
            fflush(stdout);
        }
    }
}
void register_handler() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }
}
