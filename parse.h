#ifndef PARSE_H
#define PARSE_H

#define MAXPARSE 16
#define MAXPIPE 4

#include "io.h"
#include <stdbool.h>
#include <string.h>

typedef enum {
    none_cmd,
    buildin_cmd,
    buildin_stdout_cmd,
    simple_cmd,
    pipe_cmd
} cmd_t;

cmd_t judgeCmdType(char *parsedHead);
void printParsed(char **parsed);
cmd_t parse(char *input, char *parsed[MAXPIPE][MAXPARSE]);

#endif // PARSE_H
