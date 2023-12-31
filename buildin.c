#include "buildin.h"

void exec_exit(void) {
    printf("exit\n");
    exit(0);
}
void exec_cd(char *path) {
    if (chdir(path) < 0) {
        exit_err(CD_NONE_EXIST_DIR, path);
    }
}

void exec_pwd(void) {
    char buf[1024];
    printf("%s\n", getcwd(buf, sizeof(buf)));
    fflush(stdout);
}

void execute_buildin(char **parsed) {
    if (strcmp(parsed[0], "exit") == 0) {
        exec_exit();
    } else if (strcmp(parsed[0], "cd") == 0) {
        exec_cd(parsed[1]);
    } else if (strcmp(parsed[0], "pwd") == 0) {
        exec_pwd();
    } else {
    }
}
