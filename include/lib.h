//
// Created by elias on 18/11/2025.
//

#ifndef SHOT_LIB_H
#define SHOT_LIB_H

#include <stdio.h>
#include <string.h>
#include <utils.h>
#include <constants.h>
#include <stdlib.h>
#include <command.h>
#include <tokenizer.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include <config.h>

#ifdef _WIN32
    #include <direct.h>
    #include <windows.h>
    #define getcwd _getcwd
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <pwd.h>
    #define getcwd getcwd
#endif

#endif //SHOT_LIB_H