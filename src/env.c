//
// Created by elias on 19/11/2025.
//

#include "env.h"

static env_t *session_env = NULL;

static void _init_env(void) {
    session_env = (env_t *)malloc(sizeof(env_t));
    if (!session_env) exit(1);

    char cwd_buffer[FILENAME_MAX];
    if (getcwd(cwd_buffer, sizeof(cwd_buffer)) == NULL) {
        free(session_env);
        exit(1);
    }

    char *home;
    get_home_directory(&home);
    if (home == NULL) {
        printf("Couldn't get home directory\n");
        printf("Press enter to exit...");
        getchar();
        exit(1);
    }


    session_env->cwd = strdup(cwd_buffer);
    session_env->home_dir = strdup(home);
}


env_t *get_env(void) {
    if (session_env == NULL) {
        _init_env();
    }

    return session_env;
}