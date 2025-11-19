//
// Created by elias on 18/11/2025.
//

#ifndef FLUX_TOKENIZER_H
#define FLUX_TOKENIZER_H

#include "lib.h"

typedef enum token_type_t {
    TOKEN_EOF,

    // Flux operators and groups
    TOKEN_FLUX,                 // ->
    TOKEN_START_TRANSACTION,    // [
    TOKEN_END_TRANSACTION,      // ]
    TOKEN_ERROR_HANDLER,        // !>

    // Expressions
    TOKEN_START_EXPRESSION,     // (
    TOKEN_END_EXPRESSION,       // )

    // Comparators
    TOKEN_GREATER_THAN,         // >
    TOKEN_GREATER_THAN_EQUALS,  // >=
    TOKEN_LESS_THAN,            // <
    TOKEN_LESS_THAN_EQUALS,     // <=
    TOKEN_EQUALS,               // ==
    TOKEN_NOT_EQUALS,           // !=

    // String comparators
    TOKEN_STARTS_WITH,          // %=
    TOKEN_ENDS_WITH,            // =%
    TOKEN_NOT_STARTS_WITH,      // !%=
    TOKEN_NOT_ENDS_WITH,        // !=%

    // Boolean logic
    TOKEN_AND,                  // &&
    TOKEN_OR,                   // ||
    TOKEN_NOT,                  // !

    // Bitwise operators
    TOKEN_BITWISE_AND,          // &
    TOKEN_BITWISE_OR,           // |
    TOKEN_BITWISE_NOT,          // ~

    // Arithmetic operators
    TOKEN_MODULO_OPERATOR,      // %
    TOKEN_PLUS,                 // +
    TOKEN_TIMES,                // *
    TOKEN_DIVIDE,               // /
    TOKEN_MINUS,                // -

    // Datatypes
    TOKEN_IDENTIFIER,           // ls, git, filter
    TOKEN_STRING,               // "hello world"
    TOKEN_NUMBER,               // 123, 45.6
    TOKEN_FILE_SELECTOR,        // @(*.c)
    TOKEN_BACK_REFERENCE        // $

} token_type_t;

typedef struct token_t {
    token_type_t type;
    char *value;
} token_t;

typedef struct token_list_t {
    token_t *items;
    size_t count;
    size_t capacity;
} token_list_t;

token_t get_next_token(char **input_ptr);

token_list_t create_token_list(void);
token_list_t tokenize_input(char *input);

token_t peek(const token_list_t *list);
token_t advance(token_list_t *list);

void reset_pointer(void);

int match(token_list_t *list, token_type_t type);
int is_operator(token_type_t type);

void token_list_add(token_list_t *list, token_t t);
void free_token_list(token_list_t *list);

#endif //FLUX_TOKENIZER_H