#ifndef PARSE_H
#define PARSE_H

#define MAXPARSE 10

#include "io.h"
#include<string.h>


typedef enum{
    NO_ERROR,
    NONE_EXIST_PROGRAM,
    NONE_EXIST_FILE,
    FAIL_OPEN_FILE,
    DUPLICATED_INPUT_FILE,
    DUPLICATED_OUTPUT_FILE,
    SYNTEX_ERROR,
    MISSING_PROGRAM,
    CD_NONE_EXIST_DIR,
    FORK_ERROR
}err_t;

typedef enum{
    none_cmd,
    buildin_cmd,
    simple_cmd,
    pipe_cmd
}cmd_t;


cmd_t parse(char * input, char ** parsed);


#endif // !PARSE_H
