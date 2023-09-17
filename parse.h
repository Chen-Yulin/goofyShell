#ifndef PARSE_H
#define PARSE_H

#include "io.h"

typedef enum{
    NO_ERROR,
    NO_EXIST_FILE,
    NO_EXIST_DIR,
    NO_EXEC_PROGRAM
}err_t;

typedef enum{
    ls,
    cd,
    program,
}cmd_t;

struct token{
    cmd_t type;
    int argc;
    char ** args;
};

extern cmd_t cmd;

err_t parse();


#endif // !PARSE_H
