#ifndef BUILDIN_H
#define BUILDIN_H

#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void exec_exit();
void exec_pwd();
void exec_cd(char *path);
void execute_buildin(char **parsed);

#endif // !BUILDIN_H
