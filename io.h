#ifndef IO_H
#define IO_H

#include "job.h"
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024

extern char input_buffer[BUFFER_SIZE];
extern char output_buffer[BUFFER_SIZE];

void get_cmd_input();

void show_shell_name();
void read_cmd_into_buffer(char *res_buffer);

#endif // !IO_H
