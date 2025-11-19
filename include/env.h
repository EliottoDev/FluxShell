//
// Created by elias on 19/11/2025.
//

#ifndef FLUX_ENV_H
#define FLUX_ENV_H

#include "lib.h"

typedef struct env_t {
    char *cwd;
    char *home_dir;
} env_t;

env_t *get_env(void);

#endif //FLUX_ENV_H