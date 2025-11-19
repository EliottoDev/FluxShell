//
// Created by elias on 19/11/2025.
//

#ifndef FLUX_BUILTINS_H
#define FLUX_BUILTINS_H

#include <lib.h>

typedef int (*builtin_function_t)(char**, int);

int builtin_print(char **args, int argc);
int builtin_cd(char **args, int argc);
int builtin_exit(char **args, int argc);
int builtin_ls(char **args, int argc);

typedef struct builtin_t {
    char *name;
    builtin_function_t func;
} builtin_t;

extern builtin_t builtins[];

builtin_t *find_builtin(const char *name);

#endif //FLUX_BUILTINS_H