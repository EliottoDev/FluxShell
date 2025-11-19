//
// Created by elias on 19/11/2025.
//

#ifndef FLUX_PARSER_H
#define FLUX_PARSER_H

#include "lib.h"
#include "tokenizer.h"

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

typedef struct node_t {
    node_type_t type;

    // Function
    char *cname;
    struct node_t **args;
    int argc;

    // Binary operations
    token_type_t op;
    struct node_t *left, *right;

    char *value; // Literal values
} node_t;

node_t *parse(token_list_t *tokens);
node_t *create_node(node_type_t type);
node_t *parse_flux(token_list_t* tokens);
node_t *parse_expression(token_list_t* tokens);
node_t *parse_function_or_block(token_list_t* tokens);

void print_ast(node_t *node);
void free_ast(node_t *node);
void print_ast_level(node_t* node, int level);

const char* get_op_str(token_type_t type);
void print_indent(int level);

#endif //FLUX_PARSER_H