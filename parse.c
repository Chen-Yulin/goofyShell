#include "parse.h"

void printParsed(char ** parsed){
    for (int j = 0;j<MAXPARSE;j++) {
        printf("%s ", parsed[j]);
    }
    printf("\n");
}

void clearStringArray(char ** input, int num){
    for (int i = 0; i < num; i++) {
        input[i] = "";
    }
}

void parseSpace(char * str, char ** parsed){
    int i = 0;
    while((parsed[i] = strsep(&str, " ")) != NULL && (i < MAXPARSE - 1)){
        if(strlen(parsed[i]) != 0){
            i++;
        }
    }
    for (int j = i;j<MAXPARSE;j++) {
        parsed[j] = NULL;
    }
}

cmd_t judgeCmdType(char * parsedHead){
    //printf("%s\n", parsedHead);
    if (strcmp(parsedHead, "exit") == 0) {
        return buildin_cmd;
    }else if (strcmp(parsedHead, "cd") == 0) {
        return buildin_cmd;
    }else if (strcmp(parsedHead, "pwd") == 0) {
        return buildin_cmd;
    }else{
        return simple_cmd;
    }
}

cmd_t parse(char * input, char ** parsed){
    clearStringArray(parsed, MAXPARSE);
    parseSpace(input, parsed);
    cmd_t type = judgeCmdType(parsed[0]);
    //printf("cmd type %d\n", type);
    return type; 
}
