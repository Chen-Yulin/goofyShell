#include "parse.h"

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
