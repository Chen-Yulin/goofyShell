#ifndef PROCESS_H
#define PROCESS_H

#include "parse.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

err_t execute_simple(char **parsed);

#endif // !PROCESS_H
