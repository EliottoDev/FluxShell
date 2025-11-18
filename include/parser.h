//
// Created by elias on 19/11/2025.
//

#ifndef FLUX_PARSER_H
#define FLUX_PARSER_H

#include <lib.h>

typedef enum {
    NODE_FLUX,          // A -> B
    NODE_FUNCTION,      // echo "hello world!"
    NODE_BLOCK,         // [ ... ]
    NODE_ERROR_HANDLER, // [ ... ] !> handle
    NODE_BINARY_OP,     // Binary expressions: A > B, A && B
    NODE_UNARY_OP,      // Unary expressions: !A, ~A
    NODE_LITERAL,       // Strings, numbers, selectors "@(...)"
    NODE_VAR_REF        // $ or variables
} node_type_t;

typedef struct {
    node_type_t type;

    // Function
    char *name;
    struct node_t **args;
    int argc;

    // Binary operations
    token_type_t op;
    struct node_t *left, *right;

    char *string_value; // Literal values
} node_t;

node_t *parse(token_list_t *tokens);
void print_ast(node_t *node);
void free_ast(node_t *node);

#endif //FLUX_PARSER_H