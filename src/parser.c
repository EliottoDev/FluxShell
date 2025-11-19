//
// Created by elias on 19/11/2025.
//

#include "parser.h"

node_t *create_node(const node_type_t type) {
    node_t *node = calloc(1, sizeof(node_t));
    node->type = type;
    return node;
}

node_t *parse_atom(token_list_t *tokens) {
    const token_t token = peek(tokens);

    if (match(tokens, TOKEN_NUMBER) || match(tokens, TOKEN_STRING) || match(tokens, TOKEN_FILE_SELECTOR)) {
        node_t *node = create_node(NODE_LITERAL);
        node->value = strdup(token.value);
        return node;
    }

    if (match(tokens, TOKEN_IDENTIFIER)) {
        node_t *node = create_node(NODE_VAR_REF);
        node->value = strdup(token.value);
        return node;
    }

    if (match(tokens, TOKEN_BACK_REFERENCE)) {
        node_t *node = create_node(NODE_VAR_REF);
        node->value = strdup("$");
        return node;
    }

    if (match(tokens, TOKEN_START_EXPRESSION)) {
        node_t *expr = parse_expression(tokens);
        if (!match(tokens, TOKEN_END_EXPRESSION)) {
            printf("Error: Expected end of expression \")\".\n");
            exit(1);
        }
        return expr;
    }

    return NULL;
}

node_t* parse_expression(token_list_t* tokens) {
    node_t *left = parse_atom(tokens);

    while (is_operator(peek(tokens).type)) {
        const token_t op = advance(tokens);

        node_t *bin_node = create_node(NODE_BINARY_OP);
        bin_node->op = op.type;
        bin_node->left = left;
        bin_node->right = parse_atom(tokens);
        left = bin_node;
    }

    return left;
}

node_t* parse_function_or_block(token_list_t* tokens) {
    if (match(tokens, TOKEN_START_TRANSACTION)) {
        node_t* block_node = create_node(NODE_BLOCK);
        block_node->left = parse_flux(tokens);

        if (!match(tokens, TOKEN_END_TRANSACTION)) {
            printf("Error: Expected ']' for transaction closing.\n");
        }

        if (match(tokens, TOKEN_ERROR_HANDLER)) {
            node_t* error_node = create_node(NODE_ERROR_HANDLER);
            error_node->left = block_node;
            error_node->right = parse_function_or_block(tokens);
            return error_node;
        }
        return block_node;
    }

    if (peek(tokens).type == TOKEN_IDENTIFIER || peek(tokens).type == TOKEN_FILE_SELECTOR) {

        node_t* cmd_node = create_node(NODE_FUNCTION);
        const token_t cmd_token = advance(tokens);

        cmd_node->cname = strdup(cmd_token.value);

        while (peek(tokens).type != TOKEN_FLUX &&
               peek(tokens).type != TOKEN_ERROR_HANDLER &&
               peek(tokens).type != TOKEN_END_TRANSACTION &&
               peek(tokens).type != TOKEN_EOF) {

            node_t* arg = parse_expression(tokens);
            if (arg) {
                cmd_node->argc++;
                cmd_node->args = realloc(cmd_node->args, sizeof(node_t*) * cmd_node->argc);
                cmd_node->args[cmd_node->argc - 1] = arg;
            }
            else advance(tokens);
        }
        return cmd_node;
    }

    return NULL;
}

node_t *parse_flux(token_list_t* tokens) {
    node_t* left = parse_function_or_block(tokens);

    while (match(tokens, TOKEN_FLUX)) {
        node_t* pipe_node = create_node(NODE_FLUX);
        pipe_node->left = left;
        pipe_node->right = parse_function_or_block(tokens);
        left = pipe_node;
    }

    return left;
}

node_t *parse(token_list_t *tokens) {
    reset_pointer();
    return parse_flux(tokens);
}

void print_indent(const int level) {
    for(int i = 0; i < level; i++)
        printf("  ");
}

const char* get_op_str(const token_type_t type) {
    switch(type) {
        case TOKEN_GREATER_THAN: return ">";
        case TOKEN_EQUALS: return "==";
        case TOKEN_AND: return "&&";
        case TOKEN_STARTS_WITH: return "%=";
        case TOKEN_NOT_STARTS_WITH: return "!%=";
        case TOKEN_ENDS_WITH: return "=%";
        case TOKEN_NOT_ENDS_WITH: return "!=%";
        case TOKEN_NOT_EQUALS: return "!=";
        case TOKEN_PLUS: return "+";
        case TOKEN_MINUS: return "-";
        case TOKEN_TIMES: return "*";
        case TOKEN_DIVIDE: return "/";
        case TOKEN_BITWISE_AND: return "&";
        case TOKEN_BITWISE_OR: return "|";
        case TOKEN_MODULO_OPERATOR: return "%";
        case TOKEN_BITWISE_NOT: return "~";
        case TOKEN_OR: return "||";
        case TOKEN_LESS_THAN: return "<";
        case TOKEN_LESS_THAN_EQUALS: return "<=";
        case TOKEN_GREATER_THAN_EQUALS: return ">=";
        default: return "?";
    }
}

void print_ast(node_t *node) {
    print_ast_level(node, 0);
}

void print_ast_level(node_t* node, const int level) {
    if (!node) return;
    print_indent(level);

    switch (node->type) {
        case NODE_FLUX:
            printf("FLUX (->)\n");
            print_ast_level(node->left, level+1);
            print_ast_level(node->right, level+1);
            break;
        case NODE_FUNCTION:
            printf("CMD: %s\n", node->cname);
            for(int i=0; i<node->argc; i++) {
                print_indent(level+1);
                printf("ARG[%d]:\n", i);
                print_ast_level(node->args[i], level+2);
            }
            break;
        case NODE_BLOCK:
            printf("TRANSACTION [ ... ]\n");
            print_ast_level(node->left, level+1);
            break;
        case NODE_ERROR_HANDLER:
            printf("ON_ERROR (!>)\n");
            print_ast_level(node->left, level+1);
            print_indent(level+1); printf("HANDLER:\n");
            print_ast_level(node->right, level+2);
            break;
        case NODE_BINARY_OP:
            printf("OP (%s)\n", get_op_str(node->op));
            print_ast_level(node->left, level+1);
            print_ast_level(node->right, level+1);
            break;
        case NODE_LITERAL:
            printf("LIT: %s\n", node->value);
            break;
        case NODE_VAR_REF:
            printf("REF: $\n");
            break;
        default: break;
    }
}

void free_ast(node_t* node) {
    if (!node) return;

    if (node->left) free_ast(node->left);
    if (node->right) free_ast(node->right);

    if (node->args) {
        for (int i = 0; i < node->argc; i++) {
            free_ast(node->args[i]);
        }
        free(node->args);
    }

    if (node->cname) free(node->cname);
    if (node->value) free(node->value);

    free(node);
}