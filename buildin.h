#ifndef BUILDIN_H
#define BUILDIN_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>


void exec_exit();
void exec_ls();
void exec_cd(char * path);
void execute_buildin(char ** parsed);


#endif // !BUILDIN_H
