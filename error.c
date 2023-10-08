#include "error.h"

void exit_err(err_t code, char *generater) {
    switch (code) {
    case NO_ERROR:
        break;
    case NONE_EXIST_PROGRAM:
        fputs(generater, stderr);
        fputs(": command not found\n", stderr);
        break;
    case NONE_EXIST_FILE:
        fputs(generater, stderr);
        fputs(": No such file or directory\n", stderr);
        break;
    case NO_PERMISSION:
        fputs(generater, stderr);
        fputs(": Permission denied\n", stderr);
        break;
    default:
        fputs(generater, stderr);
        fputs(": some errors occur\n", stderr);
        break;
    }
    exit(code);
}
