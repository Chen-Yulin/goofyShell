#include "handler.h"

void signal_handler(int signo) {
    if (signo == 2) {
        // exit(0);
    }
}
void register_handler() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }
}
