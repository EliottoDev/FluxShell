//
// Created by elias on 18/11/2025.
//

#include "utils.h"

void s_gets(char* str, const int n) {
    char* str_read = fgets(str, n, stdin);
    if (!str_read)
        return;

    int i = 0;
    while (str[i] != '\n' && str[i] != '\0')
        i++;

    if (str[i] == '\n')
        str[i] = '\0';
}

void creplace_str(char *str, char old_char, char new_char) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == old_char) {
            str[i] = new_char;
        }
    }
}

char *sreplace_str(char *source, char *orig, char *rep) {
    char *result;
    char *tmp;
    int count;

    if (!source || !orig)
        return NULL;
    const int len_orig = strlen(orig);
    if (len_orig == 0)
        return NULL;
    if (!rep)
        rep = "";
    const int len_rep = strlen(rep);

    const char *ins = source;
    for (count = 0; (tmp = strstr(ins, orig)); ++count) {
        ins = tmp + len_orig;
    }

    tmp = result = malloc(strlen(source) + (len_rep - len_orig) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(source, orig);
        const int len_front = ins - source;
        tmp = strncpy(tmp, source, len_front) + len_front;
        tmp = strcpy(tmp, rep) + len_rep;
        source += len_front + len_orig;
    }

    strcpy(tmp, source);

    return result;
}

void get_home_directory(char **out_homedir) {
    if (out_homedir == NULL) return;

    char *temp;

    if ((temp = getenv("HOME")) != NULL) {
        *out_homedir = temp;
        return;
    }

#ifdef _WIN32
    if ((temp = getenv("USERPROFILE")) != NULL) {
        *out_homedir = temp;
        return;
    }
#else
    struct passwd *pw = getpwuid(getuid());
    if (pw) {
        *out_homedir = pw->pw_dir;
        return;
    }
#endif

    *out_homedir = NULL;
}

char* strndup(const char* s, size_t n) {
    char* result = (char*)malloc(n + 1);
    if (result) {
        memcpy(result, s, n);
        result[n] = '\0';
    }
    return result;
}

void remove_char(char *str, char target) {
    if (str == NULL) return;

    const char *read = str;
    char *write = str;

    while (*read) {
        if (*read != target) {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
}

void get_representable_path(char *path, char *out_path) {
    env_t *env = get_env();
    char *home = env->home_dir;
    size_t home_len = strlen(home);

    const char *src = path;
    int dest_idx = 0;

    if (home && strncmp(path, home, home_len) == 0) {
        char next_char = path[home_len];

        if (next_char == '/' || next_char == '\\' || next_char == '\0') {
            out_path[dest_idx++] = '~';
            src += home_len;
        }
    }

    while (*src) {
        if (*src == '\\') {
            out_path[dest_idx++] = '/';
        } else {
            out_path[dest_idx++] = *src;
        }
        src++;
    }

    out_path[dest_idx] = '\0';
}

int list_directory(const char *raw_path) {

    char path[2048];
    strncpy(path, raw_path, sizeof(path) - 1);
    path[sizeof(path) - 1] = '\0';

    for (int i = 0; path[i]; i++) {
        if (path[i] == WRONG_SLASH) {
            path[i] = PREFERRED_SLASH;
        }
    }

    #ifdef _WIN32
        WIN32_FIND_DATA fdFile;
        HANDLE hFind = NULL;
        char search_pattern[2048];

        size_t len = strlen(path);
        int ends_with_slash = (len > 0 && path[len - 1] == PREFERRED_SLASH);

        if (ends_with_slash) {
            snprintf(search_pattern, sizeof(search_pattern), "%s*", path);
        } else {
            snprintf(search_pattern, sizeof(search_pattern), "%s\\*", path);
        }

        if ((hFind = FindFirstFile(search_pattern, &fdFile)) == INVALID_HANDLE_VALUE) {
            printf("ls: cannot access '%s': Path not found or invalid\n", raw_path);
            return 1;
        }

        do {
            if (strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
                if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    printf("[%s]  ", fdFile.cFileName);
                } else {
                    printf("%s  ", fdFile.cFileName);
                }
            }
        } while (FindNextFile(hFind, &fdFile));

        FindClose(hFind);
        printf("\n");
        return 0;
    #else
        struct dirent *dp;
        DIR *dir = opendir(path);

        if (!dir) {
            printf("ls: cannot access '%s': No such file or directory\n", raw_path);
            return 1;
        }

        while ((dp = readdir(dir)) != NULL) {
            if (dp->d_name[0] != '.') {
                printf("%s  ", dp->d_name);
            }
        }

        closedir(dir);
        printf("\n");
        return 0;
    #endif
}