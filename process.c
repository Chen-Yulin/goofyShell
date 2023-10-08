#include "process.h"

void close_all_pipe(int pipefd[MAXPIPE - 1][2]) {
    for (int i = 0; i < MAXPIPE - 1; i++) {
        close(pipefd[i][READ]);
        close(pipefd[i][WRITE]);
    }
}

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

bool validateFileName(char *name) {
    if (name == NULL) {
        return false;
    }
    if (strcmp(name, ">") == 0) {
        return false;
    } else if (strcmp(name, ">>") == 0) {
        return false;
    } else if (strcmp(name, "<") == 0) {
        return false;
    }
    return true;
}

bool CheckEmptyCommand(char *parsed[MAXPIPE][MAXPARSE]) {
    bool detectEmpty = false;
    for (int i = 0; i < MAXPIPE; i++) {
        if (!detectEmpty && parsed[i][0] == NULL) {
            detectEmpty = true;
        } else if (detectEmpty && parsed[i][0] != NULL) {
            exit_err(MISSING_PIPE_PROGRAM, "error");
            return false;
        }
    }
    return true;
}

err_t execute_cmd(char **parsed, bool pipeIn, bool pipeOut) {
    char *argv[MAXPARSE] = {NULL};
    int argc = 0;
    int redirected = fetchCommand(parsed, argv);

    // solve redirection
    if (redirected) {
        int index = argc;

        int inputCnt = pipeIn ? 1 : 0;
        int outputCnt = pipeOut ? 1 : 0;
        while (parsed[index] != NULL) {
            if (strcmp(parsed[index], ">") == 0) {
                if (index == 0) {
                    exit_err(MISSING_PROGRAM, "error");
                }
                index++;
                outputCnt++;

                char *fileName = parsed[index];
                if (validateFileName(fileName)) {
                    if (outputCnt > 1) {
                        exit_err(DUPLICATED_OUTPUT_FILE, "error");
                    }

                    // printf("%s\n", fileName);
                    int fd =
                        open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                    if (fd < 0) {
                        exit_err(NO_PERMISSION, fileName);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                } else {
                    if (fileName) {
                        exit_err(SYNTEX_ERROR, fileName);
                    } else if (pipeOut) {
                        exit_err(SYNTEX_ERROR, "|");
                    }
                }
            } else if (strcmp(parsed[index], "<") == 0) {
                if (index == 0) {
                    exit_err(MISSING_PROGRAM, "error");
                }
                index++;
                inputCnt++;

                char *fileName = parsed[index];
                if (validateFileName(fileName)) {
                    if (inputCnt > 1) {
                        exit_err(DUPLICATED_INPUT_FILE, "error");
                    }

                    // printf("%s\n", fileName);
                    int fd = open(fileName, O_RDONLY);
                    if (fd < 0) {
                        exit_err(NONE_EXIST_FILE, fileName);
                    }
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                } else {
                    if (fileName) {
                        exit_err(SYNTEX_ERROR, fileName);
                    } else if (pipeOut) {
                        exit_err(SYNTEX_ERROR, "|");
                    }
                }

            } else if (strcmp(parsed[index], ">>") == 0) {
                if (index == 0) {
                    exit_err(MISSING_PROGRAM, "error");
                }
                index++;
                outputCnt++;

                // printf("append\n");
                char *fileName = parsed[index];
                if (validateFileName(fileName)) {
                    if (outputCnt > 1) {
                        exit_err(DUPLICATED_OUTPUT_FILE, "error");
                    }

                    // printf("%s\n", fileName);
                    int fd =
                        open(fileName, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
                    if (fd < 0) {
                        exit_err(NO_PERMISSION, fileName);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                } else {
                    if (fileName) {
                        exit_err(SYNTEX_ERROR, fileName);
                    } else if (pipeOut) {
                        exit_err(SYNTEX_ERROR, "|");
                    }
                }

            } else {
                index++;
            }
        }
    }

    // excute command
    cmd_t type = judgeCmdType(argv[0]);
    switch (type) {
    case buildin_stdout_cmd:
        execute_buildin(argv);
        exit(0);
        break;
    case simple_cmd:
        if (execvp(argv[0], argv) < 0) {
            exit_err(NONE_EXIST_PROGRAM, argv[0]);
            return NONE_EXIST_PROGRAM;
        }
        break;
    default:
        break;
    }
    return NO_ERROR;
}

void RecursiveCreateChild(int total, int current, int pipefd[MAXPIPE - 1][2],
                          char *parsed[MAXPIPE][MAXPARSE]) {
    if (current == total) {
        // close_all_pipe(pipefd);
        for (int i = 0; i < total; i++) {
            wait(NULL);
            jobNum--;
            printJobNum();
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
        bool pipeIn = false;
        bool pipeOut = false;
        if (current < total - 1) {
            pipeOut = true;
            dup2(pipefd[current][WRITE], STDOUT_FILENO);
            close(pipefd[current][READ]);
        }
        if (current > 0) {
            pipeIn = true;
            dup2(pipefd[current - 1][READ], STDIN_FILENO);
            close(pipefd[current - 1][WRITE]);
        }
        execute_cmd(parsed[current], pipeIn, pipeOut);
    } else {
        jobNum++;
        printJobNum();
        if (current < total - 1) {
            close(pipefd[current][WRITE]);
        }
        if (current > 0) {
            close(pipefd[current - 1][READ]);
        }
        RecursiveCreateChild(total, current + 1, pipefd, parsed);
        return;
    }
}

err_t execute_simple(char **parsed) {
    if (parsed[0] == NULL) {
        return NO_ERROR;
    }

    // create new process
    jobNum++;
    printJobNum();
    pid_t pid = fork();
    if (pid == -1) {
        return OTHER_ERROR;
    } else if (pid == 0) {
        execute_cmd(parsed, false, false);
        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL);
        jobNum--;
        printJobNum();
        return NO_ERROR;
    }
}

err_t execute_pipe(char *parsed[MAXPIPE][MAXPARSE]) {
    if (CheckEmptyCommand(parsed)) {
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
    }

    return NO_ERROR;
}
