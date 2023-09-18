#include "process.h"


err_t execute_simple(char ** parsed){
    // create new process
	pid_t pid = fork();

	if (pid == -1) {
        return FORK_ERROR;
	} else if (pid == 0) {
		if (execvp(parsed[0], parsed) < 0) {
            return NONE_EXIST_PROGRAM;
		}
		exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return NO_ERROR;
	}
}
