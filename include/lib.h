//
// Created by elias on 18/11/2025.
//

#ifndef FLUX_LIB_H
#define FLUX_LIB_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include <config.h>

#include <utils.h>
#include <constants.h>
#include <builtins.h>
#include <env.h>

#ifdef _WIN32
    #include <direct.h>
    #include <windows.h>
    #define getcwd _getcwd
    #define chdir _chdir
    #define PREFERRED_SLASH '\\'
    #define WRONG_SLASH '/'
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <pwd.h>
    #include <sys/wait.h>
    #define getcwd getcwd
    #define PREFERRED_SLASH '/'
    #define WRONG_SLASH '\\'
#endif

#endif //FLUX_LIB_H