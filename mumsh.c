#include "mumsh.h"


int main(){
    char* parsedArgs[MAXPARSE];
    while(1){
        get_cmd_input();
        cmd_t type = parse(input_buffer, parsedArgs);
        switch (type) {
            case buildin_cmd:
                printf("exec buildin_cmd\n");
                execute_buildin(parsedArgs);
                break;
            default:
                break;
        }

    }
    return 0;
}


