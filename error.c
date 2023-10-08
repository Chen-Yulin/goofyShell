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
    case DUPLICATED_INPUT_FILE:
        fputs(generater, stderr);
        fputs(": duplicated input redirection\n", stderr);
        break;
    case DUPLICATED_OUTPUT_FILE:
        fputs(generater, stderr);
        fputs(": duplicated input redirection\n", stderr);
        break;
    case SYNTEX_ERROR:
        fputs("syntax error near unexpected token `", stderr);
        fputs(generater, stderr);
        fputs("`\n", stderr);
        break;
    case MISSING_PROGRAM:
        fputs(generater, stderr);
        fputs(": missing program\n", stderr);
        break;
    case MISSING_PIPE_PROGRAM:
        fputs(generater, stderr);
        fputs(": missing program\n", stderr);
        return;
    case CD_NONE_EXIST_DIR:
        fputs(generater, stderr);
        fputs(": No such file or directory\n", stderr);
        return;
    default:
        fputs(generater, stderr);
        fputs(": some errors occur\n", stderr);
        break;
    }
    exit((int)code);
}
