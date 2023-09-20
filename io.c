#include "io.h"

char input_buffer[BUFFER_SIZE];
char output_buffer[BUFFER_SIZE];

void clear_input_buffer(){
    for (int i = 0; i < BUFFER_SIZE; i++) {
        input_buffer[i] = '\0';
    }
}

void show_shell_name(){
    printf("mumsh $ ");
    fflush(stdout);
}

void read_cmd_into_buffer(char* res_buffer){
    int index = 0;
    char c = 0;
    while((c = (char)getchar())){
        if (c == EOF){
            //exit
        }
        if (c == '\n'){
            res_buffer[index++] = '\0';
            break;
        }else{
            res_buffer[index++] = c;
        }
    }
}

void get_cmd_input(){
    clear_input_buffer();
    show_shell_name();
    read_cmd_into_buffer(input_buffer);
}
