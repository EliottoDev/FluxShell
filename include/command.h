//
// Created by elias on 18/11/2025.
//

#ifndef SHOT_COMMAND_H
#define SHOT_COMMAND_H

typedef int (*cmd_func_t)(char **args);

typedef struct {
    const char *name;
    const char** aliases;
    const char *description;
    cmd_func_t func;
} command_t;

#endif //SHOT_COMMAND_H