#include <lib.h>

#include "executor.h"
#include "parser.h"
#include "tokenizer.h"

#define SHOW_TOKENS false
#define SHOW_AST false

int main(void) {

#ifndef PROJECT_VERSION
    printf("Couldn't retrieve project's version!\n");
    exit(1)
#endif

    char console_input[MAX_USER_INPUT];

    const env_t *session_env = get_env();

    printf("Flux Shell v%s\n\n", PROJECT_VERSION);

    while (1) {
        char repr_cwd[FILENAME_MAX];
        get_representable_path(session_env->cwd, repr_cwd);

        printf("%s %s", repr_cwd, PREFIX);

        fflush(stdout);

        s_gets(console_input, MAX_USER_INPUT);

        token_list_t tokens = tokenize_input(console_input);
        if (SHOW_TOKENS) {
            for (size_t i = 0; i < tokens.count; i++) {
                printf("[%zu] Type: 0x%01x, Value: %s\n",
                       i,
                       tokens.items[i].type,
                       tokens.items[i].value ? tokens.items[i].value : "NULL");
            }
        }

        if (tokens.count > 0) {
            node_t *ast = parse(&tokens);

            if (ast != NULL) {
                if (SHOW_AST) {
                    printf("Debug: AST generated:\n");
                    print_ast(ast);
                }

                execute(ast);
                free_ast(ast);
            }
        }

        free_token_list(&tokens);
    }

    printf("EOP Signal\n");
    fgetc(stdin);
    return 0;
}
