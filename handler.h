#ifndef HANDLER_H
#define HANDLER_H

#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

void signal_handler(int signo);
void register_handler();

#endif // !HANDLER_H
