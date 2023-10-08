#ifndef ERR_H
#define ERR_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    NO_ERROR,
    NONE_EXIST_PROGRAM,     // case 1
    NONE_EXIST_FILE,        // case 2
    NO_PERMISSION,          // case 3
    DUPLICATED_INPUT_FILE,  // case 4
    DUPLICATED_OUTPUT_FILE, // case 5
    SYNTEX_ERROR,           // case 6
    MISSING_PROGRAM,        // case 7
    CD_NONE_EXIST_DIR,      // case 8
    OTHER_ERROR
} err_t;

void exit_err(err_t code, char *generater);

#endif // !ERR_H
