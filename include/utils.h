//
// Created by elias on 18/11/2025.
//

#ifndef FLUX_UTILS_H
#define FLUX_UTILS_H

#include <lib.h>

void s_gets(char *str, int n);
void creplace_str(char *str, char old_char, char new_char);
char *sreplace_str(char *source, char *orig, char *rep);
void get_home_directory(const char **out_homedir);
char* strndup(const char* s, size_t n);
void remove_char(char *str, char target);

#endif //FLUX_UTILS_H