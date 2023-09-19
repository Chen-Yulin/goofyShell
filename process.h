#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>
#include "parse.h"

err_t execute_simple(char ** parsed);


#endif // !PROCESS_H
