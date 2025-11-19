//
// Created by elias on 19/11/2025.
//

#ifndef FLUX_EXECUTOR_H
#define FLUX_EXECUTOR_H

#include <lib.h>
#include "parser.h"

char* evaluate_to_string(node_t* node);
char* build_windows_command_line(const char* cmd, char** argv, const int count);
int execute_function_node(const node_t* node);
int exec_external_program(char* command, char** args, const int arg_count);
int execute(node_t* node);

#endif //FLUX_EXECUTOR_H