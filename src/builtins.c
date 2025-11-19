//
// Created by elias on 19/11/2025.
//

#include "builtins.h"

builtin_t builtins[] = {
    {"print"    , builtin_print },
    {"cd"       , builtin_cd    },
    {"exit"     , builtin_exit  },
    {"ls"       , builtin_ls    },
    {NULL       , NULL          } // NULL as an end delimiter (trash way)
};

int builtin_print(char **args, int argc) {
    for (int i = 0; i < argc; i++) {
        printf("%s", args[i]);
        if (i < argc - 1) printf(" ");
    }
    printf("\n");
    return 0;
}

int builtin_cd(char **args, int argc) {
    if (argc < 1 ) {
        printf("Execution Error: 'cd' requires a path parameter.\n");
        return 1;
    }
// TODO: Handle directory change.
    if (chdir(args[0]) != 0) {
        perror("");
        return 1;
    }
    return 0;
}

int builtin_exit(char **args, int argc) {
    printf("Exiting Flux.");
    exit(0);
}


builtin_t *find_builtin(const char *name) {
    for (int i = 0; builtins[i].name != NULL; i++) {
        if (strcmp(name, builtins[i].name) == 0) {
            return (builtins + i);
        }
    }
    return NULL;
}

int builtin_ls(char **args, int argc) {
    env_t *env = get_env();

    if (argc == 0) {
        return list_directory(env->cwd);
    }

    int status = 0;
    for (int i = 0; i < argc; i++) {
        if (argc > 1) printf("%s:\n", args[i]);

        if (list_directory(args[i]) != 0) {
            status = 1;
        }

        if (argc > 1 && i < argc - 1) printf("\n");
    }

    return status;
}