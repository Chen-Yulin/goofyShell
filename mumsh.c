#include "mumsh.h"

int main(void) {
    char *parsedArgs[MAXPIPE][MAXPARSE];
    register_handler();
    while (1) {
        get_cmd_input();
        cmd_t type = parse(input_buffer, parsedArgs);
        switch (type) {
        case buildin_cmd:
            // printf("exec buildin_cmd\n");
            execute_buildin(parsedArgs[0]);
            break;
        case simple_cmd:
            execute_simple(parsedArgs[0]);
            break;
        case buildin_stdout_cmd:
            execute_simple(parsedArgs[0]);
            break;
        case pipe_cmd:
            execute_pipe(parsedArgs);
            break;
        default:
            break;
        }
    }
    return 0;
}
