#include "io.h"

char input_buffer[BUFFER_SIZE];
char output_buffer[BUFFER_SIZE];

void clear_input_buffer(void) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        input_buffer[i] = '\0';
    }
}

void show_shell_name(void) {
    fflush(stdout);
    printf("mumsh $ ");
    fflush(stdout);
}

void read_cmd_into_buffer(char *res_buffer) {
    int index = 0;
    int c = 0;
    while ((c = getchar())) {
        if (c == EOF) {
            if (index == 0) {
                // exit
                printf("\nexit\n");
                exit(0);
            } else {
                break;
            }
        } else if (c == '\n') {
            res_buffer[index++] = '\0';
            break;
        } else {
            res_buffer[index++] = (char)c;
        }
    }
}

void get_cmd_input(void) {
    clear_input_buffer();
    show_shell_name();
    read_cmd_into_buffer(input_buffer);
}
