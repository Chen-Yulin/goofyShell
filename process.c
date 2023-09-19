#include "process.h"

bool isRedirectionSign(char* c){
    if (c !=NULL) {
        return strcmp(c, "<") == 0 || strcmp(c, ">") == 0 || strcmp(c, ">>") == 0;
    }else{
        return false;
    }
}

err_t execute_simple(char ** parsed){
    // split the part without redirection sign
    char * argv[MAXPARSE] = {NULL};
    int argc = 0;
    int redirected = false;
    while (argc < MAXPARSE-1) {
        if (isRedirectionSign(parsed[argc])) {
            redirected = true;
            break;
        }
        argv[argc] = parsed[argc];
        argc++;
    }
    //printParsed(argv);
    
    // create new process
    pid_t pid = fork();
	if (pid == -1) {
        return FORK_ERROR;
	} else if (pid == 0) {
        // solve redirection
        if (redirected) {
            int index = argc;
            while(parsed[index]!= NULL){
                if (strcmp(parsed[index], ">") == 0) {
                    index++;
                    char * fileName = parsed[index];
                    //printf("%s\n", fileName);
                    int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }else if (strcmp(parsed[index], "<") == 0) {
                    index++;
                    char * fileName = parsed[index];
                    //printf("%s\n", fileName);
                    int fd = open(fileName, O_RDONLY);
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }else if (strcmp(parsed[index], ">>") == 0) {
                    index++;
                    //printf("append\n");
                    char * fileName = parsed[index];
                    //printf("%s\n", fileName);
                    int fd = open(fileName, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }else{
                    index++;
                }
            }
        }


        // excute command
		if (execvp(argv[0], argv) < 0) {
            return NONE_EXIST_PROGRAM;
		}
		exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return NO_ERROR;
	}
}
