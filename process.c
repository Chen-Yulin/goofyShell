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
    // 0 is read end, 1 is write end
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return OTHER_ERROR;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCould not fork");
        return OTHER_ERROR;
    }

    if (p1 == 0) {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(parsed[0][0], parsed[0]) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    } else {
        // Parent executing
        p2 = fork();

        if (p2 < 0) {
            printf("\nCould not fork");
            return OTHER_ERROR;
        }

        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsed[1][0], parsed[1]) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        } else {
            // parent executing, waiting for two children
            wait(NULL);
            wait(NULL);
        }
    }
    return NO_ERROR;
}
