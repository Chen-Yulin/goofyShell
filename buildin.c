#include "buildin.h"

void exec_exit(){
    exit(0);
}
void exec_cd(char * path){
    chdir(path);
}

void execute_buildin(char **parsed){
    if (strcmp(parsed[0], "exit") == 0) {
        exec_exit();
    }else if (strcmp(parsed[0], "cd") == 0) {
        printf("cd to %s\n", parsed[1]);
        exec_cd(parsed[1]);
    }else if (strcmp(parsed[0], "ls") == 0) {
    }else{
    }
}
