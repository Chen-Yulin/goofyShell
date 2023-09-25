#include "parse.h"

void printParsedPiped(char *pipe[MAXPIPE][MAXPARSE], int cnt) {
    for (int i = 0; i < cnt; i++) {
        printParsed(pipe[i]);
    }
}

void printParsed(char **parsed) {
    for (int i = 0; i < MAXPARSE; i++) {
        if (parsed[i] != NULL) {
            printf("%s ", parsed[i]);
        } else {
            printf("null ");
        }
    }
    printf("\n");
}

void clearStringArray(char **input, int num) {
    for (int i = 0; i < num; i++) {
        input[i] = "";
    }
}

void clearParsed(char *parsed[MAXPIPE][MAXPARSE]) {
    for (int i = 1; i <= MAXPIPE; i++) {
        parsed[i][0] = NULL;
    }
}

void stringSag(char *str, int num) {
    for (int i = num; i > 0; i--) {
        str[i] = str[i - 1];
    }
}

void insertSpace(char *str, int index, int len, bool left) {
    if (left) {
        stringSag(str + index, len - index);
        str[index] = ' ';
    } else {
        stringSag(str + index + 1, len - index - 1);
        str[index + 1] = ' ';
    }
}

void addMissingSpace(char *input) {
    int len = (int)strlen(input);
    for (int i = 0; i < len; i++) {
        if (input[i] == '<') {
            insertSpace(input, i, len, true);
            len++;
            i++;
            insertSpace(input, i, len, false);
            len++;
            i++;
        } else if (input[i] == '>') {
            bool expandleft = true;
            bool expandright = true;
            if (i - 1 >= 0) {
                if (input[i - 1] == '>') {
                    expandleft = false;
                }
            }
            if (i + 1 < BUFFER_SIZE) {
                if (input[i + 1] == '>') {
                    expandright = false;
                }
            }

            if (expandleft) {
                insertSpace(input, i, len, true);
                len++;
                i++;
            }
            if (expandright) {
                insertSpace(input, i, len, false);
                len++;
                i++;
            }
        }
        if (len > BUFFER_SIZE) {
            len = BUFFER_SIZE;
        }
    }
    // printf("%s\n", input);
}

void parseSpace(char *str, char **parsed) {
    int i = 0;
    while ((parsed[i] = strsep(&str, " ")) != NULL && (i < MAXPARSE - 1)) {
        if (strlen(parsed[i]) != 0) {
            i++;
        }
    }
    for (int j = i; j < MAXPARSE; j++) {
        parsed[j] = NULL;
    }
}
int parsePipe(char *str, char **piped) {
    int i;
    for (i = 0; i < MAXPIPE; i++) {
        piped[i] = strsep(&str, "|");
        if (piped[i] == NULL)
            break;
    }

    return i;
}

cmd_t judgeCmdType(char *parsedHead) {
    if (parsedHead == NULL) {
        return none_cmd;
    }
    if (strcmp(parsedHead, "exit") == 0) {
        return buildin_cmd;
    } else if (strcmp(parsedHead, "cd") == 0) {
        return buildin_cmd;
    } else if (strcmp(parsedHead, "pwd") == 0) {
        return buildin_cmd;
    } else {
        return simple_cmd;
    }
}

cmd_t parse(char *input, char *parsed[MAXPIPE][MAXPARSE]) {
    clearStringArray(parsed[0], MAXPARSE);
    clearParsed(parsed);
    addMissingSpace(input);

    char *pipedInput[MAXPIPE];
    int pipeCnt = parsePipe(input, pipedInput);
    // printf("%d\n", pipeCnt);

    for (int i = 0; i < pipeCnt; i++) {
        parseSpace(pipedInput[i], parsed[i]);
    }

    // printParsedPiped(parsed, pipeCnt);
    cmd_t type = none_cmd;
    if (pipeCnt > 1) {
        return pipe_cmd;
    } else {
        type = judgeCmdType(parsed[0][0]);
    }
    return type;
}
