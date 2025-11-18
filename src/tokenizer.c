//
// Created by elias on 18/11/2025.
//

#include "tokenizer.h"

// test: @(*.c) -> filter($ !%= "test_")  -> [ compile $ -> link $ ] !> print "Error while compiling and linking" -> print "Compilation and linking done"

static size_t list_pointer = 0;

token_t get_next_token(char **input_ptr) {
    char *current = *input_ptr;
    token_t token = {0};
    while (isspace(*current)) current++;

    if (*current == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    switch (*current) {
        case '-':
            if (*(current + 1) == '>') {
                token.type = TOKEN_FLUX;
                token.value = strndup("->", 2);
                current += 2;
            } else {
                token.type = TOKEN_MINUS;
                token.value = strndup("-", 1);
                current++;
            }
            break;
        case '[':
            token.type = TOKEN_START_TRANSACTION;
            token.value = strndup("[", 1);
            current++;
            break;
        case ']':
            token.type = TOKEN_END_TRANSACTION;
            token.value = strndup("]", 1);
            current++;
            break;
        case '(':
            token.type = TOKEN_START_EXPRESSION;
            token.value = strndup("(", 1);
            current++;
            break;
        case ')':
            token.type = TOKEN_END_EXPRESSION;
            token.value = strndup(")", 1);
            current++;
            break;
        case '>':
            if (*(current + 1) == '=') {
                token.type = TOKEN_GREATER_THAN_EQUALS;
                token.value = strndup(">=", 2);
                current += 2;
            } else {
                token.type = TOKEN_GREATER_THAN;
                token.value = strndup(">", 1);
                current++;
            }
            break;
        case '<':
            if (*(current + 1) == '=') {
                token.type = TOKEN_LESS_THAN_EQUALS;
                token.value = strndup("<=", 2);
                current += 2;
            } else {
                token.type = TOKEN_LESS_THAN;
                token.value = strndup("<", 1);
                current++;
            }
        case '!':
            if (*(current + 1) == '=') {
                token.type = TOKEN_NOT_EQUALS;
                token.value = strndup("!=", 2);
                current += 2;
            } else if (*(current + 1) == '>') {
                token.type = TOKEN_ERROR_HANDLER;
                token.value = strndup("!>", 2);
                current += 2;
            } else if (*(current + 1) == '%' && *(current + 2) == '='){
                token.type = TOKEN_NOT_STARTS_WITH;
                token.value = strndup("!%=", 3);
                current += 3;
            } else if (*(current + 2) == '%' && *(current + 1) == '=') {
                token.type = TOKEN_NOT_ENDS_WITH;
                token.value = strndup("!=%", 3);
                current += 3;
            } else {
                token.type = TOKEN_NOT;
                token.value = strndup("!", 1);
                current++;
            }
            break;
        case '&':
            if (*(current + 1) == '&') {
                token.type = TOKEN_AND;
                token.value = strndup("&&", 2);
                current += 2;
            } else {
                token.type = TOKEN_BITWISE_AND;
                token.value = strndup("&", 1);
                current++;
            }
            break;
        case '|':
            if (*(current + 1) == '|') {
                token.type = TOKEN_OR;
                token.value = strndup("||", 2);
                current += 2;
            } else {
                token.type = TOKEN_BITWISE_OR;
                token.value = strndup("|", 1);
                current++;
            }
            break;
        case '=':
            if (*(current + 1) == '=') {
                token.type = TOKEN_EQUALS;
                token.value = strndup("==", 2);
                current += 2;
            } else if (*(current + 1) == '%') {
                token.type = TOKEN_ENDS_WITH;
                token.value = strndup("=%", 2);
                current += 2;
            }
            break;
        case '%':
            if (*(current + 1) == '=') {
                token.type = TOKEN_STARTS_WITH;
                token.value = strndup("%=", 2);
                current += 2;
            } else {
                token.type = TOKEN_MODULO_OPERATOR;
                token.value = strndup("%", 1);
                current++;
            }
            break;
        case '~':
            token.type = TOKEN_BITWISE_NOT;
            token.value = strndup("~", 1);
            current++;
            break;
        case '@':
            if (*(current + 1) == '(') {
                ++current;
                const char *start = ++current;
                while (*current != ')') current++;
                const uint16_t length = current - start;
                token.type = TOKEN_FILE_SELECTOR;
                token.value = strndup(start, length);
                current++;
            }
            break;
        case '\"':
            const char *start = ++current;
            while (*current != '\"') current++;
            const uint16_t length = current - start;
            token.type = TOKEN_STRING;
            token.value = strndup(start, length);
            current++;
            break;
        case '$':
            token.type = TOKEN_BACK_REFERENCE;
            token.value = strndup("$", 1);
            current++;
            break;
        default:
            if (isalpha(*current)) {
                const char *start = current;
                while (isalnum(*current) || *current == '_') current++;

                const uint16_t length = current - start;

                token.type = TOKEN_IDENTIFIER;
                token.value = strndup(start, length);
            }
            if (isdigit(*current)) {
                char *start = current;
                bool has_decimal = false;
                while (isdigit(*current) || *current == '_' || *current == '.') current++;

                const uint16_t length = current - start;

                token.type = TOKEN_NUMBER;
                token.value = strndup(start, length);
                remove_char(token.value, '_');
            }
            break;
    }

    *input_ptr = current;
    return token;
}

token_list_t create_token_list(void) {
    token_list_t list = {
        .capacity = DEFAULT_TOKEN_LIST_CAPACITY,
        .count = 0,
        .items = calloc(DEFAULT_TOKEN_LIST_CAPACITY, sizeof(token_t))
    };

    return list;
}

void token_list_add(token_list_t *list, token_t t) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        if ((list->items = realloc(list->items, sizeof(token_t) * list->capacity)) == NULL) {
            printf("Error reallocating token list\n");
            exit(1);
        }
    }
    list->items[list->count++] = t;
}

void free_token_list(token_list_t *list) {
    for (size_t i = 0; i < list->count; i++) {
        if (list->items[i].value) {
            free(list->items[i].value);
        }
    }
    free(list->items);
    list->count = 0;
}

token_list_t tokenize_input(char *input) {
    token_list_t list = create_token_list();
    char *cursor = input;

    while (1) {
        token_t t = get_next_token(&cursor);

        token_list_add(&list, t);

        if (t.type == TOKEN_EOF) {
            break;
        }
    }

    return list;
}

token_t peek(const token_list_t *list) {
    if (list_pointer >= list->count)
        return (token_t){TOKEN_EOF, NULL};
    return list->items[list_pointer];
}

token_t advance(token_list_t *list) {
    if (list_pointer < list->count)
        list_pointer++;
    return list->items[list_pointer - 1];
}

void reset_pointer(void) {
    list_pointer = 0;
}

int match(token_list_t *list, token_type_t type) {
    if (peek(list).type == type) {
        advance(list);
        return 1;
    }
    return 0;
}

int is_operator(token_type_t type) {
    return type >= TOKEN_GREATER_THAN && type <= TOKEN_MINUS;
}