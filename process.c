#include "process.h"

bool isRedirectionSign(char *c) {
    if (c != NULL) {
        return strcmp(c, "<") == 0 || strcmp(c, ">") == 0 ||
               strcmp(c, ">>") == 0;
    } else {
        return false;
    }
}

bool fetchCommand(char **parsed, char **argv) {
    int ip = 0;
    int ic = 0;
    bool redirected = false;
    while (parsed[ip] != NULL) {
        if (isRedirectionSign(parsed[ip])) {
            redirected = true;
            ip += 2;
        } else {
            argv[ic] = parsed[ip];
            ic++;
            ip++;
        }
    }
    return redirected;
}
err_t execute_cmd(char **parsed) {
    char *argv[MAXPARSE] = {NULL};
    int argc = 0;
    int redirected = fetchCommand(parsed, argv);

    // solve redirection
    if (redirected) {
        int index = argc;
        while (parsed[index] != NULL) {
            if (strcmp(parsed[index], ">") == 0) {
                index++;
                char *fileName = parsed[index];
                // printf("%s\n", fileName);
                int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            } else if (strcmp(parsed[index], "<") == 0) {
                index++;
                char *fileName = parsed[index];
                // printf("%s\n", fileName);
                int fd = open(fileName, O_RDONLY);
                dup2(fd, STDIN_FILENO);
                close(fd);
            } else if (strcmp(parsed[index], ">>") == 0) {
                index++;
                // printf("append\n");
                char *fileName = parsed[index];
                // printf("%s\n", fileName);
                int fd = open(fileName, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            } else {
                index++;
            }
        }
    }

    // excute command
    if (execvp(argv[0], argv) < 0) {
        return NONE_EXIST_PROGRAM;
    }
    return NO_ERROR;
}

void RecursiveCreateChild(int total, int current, int pipefd[MAXPIPE - 1][2],
                          char *parsed[MAXPIPE][MAXPARSE]) {
    if (current == total) {
        for (int i = 0; i < total; i++) {
            wait(NULL);
        }
        return;
    }
    pid_t p = fork();
    if (p < 0) {
        printf("\nCould not fork\n");
        return;
    }
    if (p == 0) {
        // child
        if (current < total - 1) {
            dup2(pipefd[current][1], STDOUT_FILENO);
        }
        if (current > 0) {
            dup2(pipefd[current - 1][0], STDIN_FILENO);
        }
        execute_cmd(parsed[current]);
    } else {
        RecursiveCreateChild(total, current + 1, pipefd, parsed);
        return;
    }
}

err_t execute_simple(char **parsed) {
    if (parsed[0] == NULL) {
        return NO_ERROR;
    }

    // create new process
    pid_t pid = fork();
    if (pid == -1) {
        return FORK_ERROR;
    } else if (pid == 0) {
        execute_cmd(parsed);
        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL);
        return NO_ERROR;
    }
}

err_t execute_pipe(char *parsed[MAXPIPE][MAXPARSE]) {
    int pipeCnt = 0;
    for (int i = 0; i < MAXPIPE; i++) {
        if (parsed[i][0] != NULL) {
            pipeCnt++;
        } else {
            break;
        }
    }
    int pipefd[MAXPIPE - 1][2];
    for (int i = 0; i < pipeCnt - 1; i++) {
        if (pipe(pipefd[i]) < 0) {
            return OTHER_ERROR;
        }
    }
    RecursiveCreateChild(pipeCnt, 0, pipefd, parsed);

    return NO_ERROR;
}
