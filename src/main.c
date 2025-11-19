#include <lib.h>

#include "parser.h"
#include "tokenizer.h"

int main(void) {

#ifndef PROJECT_VERSION
    printf("Couldn't retrieve project's version!\n");
    exit(1)
#endif

    char console_input[MAX_USER_INPUT];

    char raw_cwd[FILENAME_MAX];

    if (getcwd(raw_cwd, sizeof(raw_cwd)) == NULL) {
        printf("Error getting current working directory\n");
        return 1;
    }

    const char *home_dir = NULL;
    get_home_directory(&home_dir);

    if (home_dir == NULL) {
        printf("Error getting home directory\n");
        return 1;
    }

    char *cwd = sreplace_str(raw_cwd, home_dir, "~");
    creplace_str(cwd, '\\', '/');

    printf("Flux Shell v%s\n\n", PROJECT_VERSION);

    while (1) {
        printf("%s %s", cwd, PREFIX);

        fflush(stdout);

        s_gets(console_input, MAX_USER_INPUT);

        if (strcmp(console_input, "exit") == 0) {
            break;
        }

        token_list_t tokens = tokenize_input(console_input);

        for (size_t i = 0; i < tokens.count; i++) {
            printf("[%zu] Type: 0x%01x, Value: %s\n",
                   i,
                   tokens.items[i].type,
                   tokens.items[i].value ? tokens.items[i].value : "NULL");
        }

        if (tokens.count > 0) {
            node_t *ast = parse(&tokens);

            print_ast(ast);

            free_ast(ast);
        }

        free_token_list(&tokens);
    }

    printf("EOP Signal\n");
    fgetc(stdin);
    return 0;
}
