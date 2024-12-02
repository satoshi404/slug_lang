#define _SLUG_LEXER_DEBUG
#include "slug_lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

char* slug_kind_to_string(Token token) {
    switch (token.selfKind) {
        case TOKEN_IDENTIFIER: return "Identifier"; break;
        case TOKEN_KEYWORD_LET: return "Let"; break;
        case TOKEN_LIT_INT: return "Integer"; break;
        case TOKEN_LIT_STRING: return "String"; break;
        case TOKEN_SEMICOLON: return "Semicolon"; break;
        case TOKEN_ASSIGN: return "Assign"; break;
        case TOKEN_COMMA: return "Comma"; break;
        case TOKEN_LPAREN: return "Left Parenthesis"; break;
        case TOKEN_RPAREN: return "Right Parenthesis"; break;
        case TOKEN_LBRACE: return "Left Brace"; break;
        case TOKEN_RBRACE: return "Right Brace"; break;
        case TOKEN_PLUS: return "Plus"; break;
        default: return "Eof"; break;
    }
}


static Token slug_lexer_create_token(TokenKind kind, int line, int column, char* id, char* lit_string, int32_t lit_i32) {
    Token token = {0};
    token.selfKind = kind;
    token.line = line;
    token.ident = id ? strdup(id) : NULL;
    token.column = column;
    if (lit_string != NULL) {
        token.literal.lit_string = strdup(lit_string);
    } else {
        token.literal.lit_i32 = lit_i32;
    }
    return token;
}

static void slug_list_alloc_token(TokenList* list, Token token) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * sizeof(Token));
    }
    list->data[list->size++] = token;
}

static TokenKind slug_lexer_get_symbol(char* input[], int line, int* column) {
    switch (input[line][*column]) {
        case '{': (*column)++; return TOKEN_LBRACE;
        case '}': (*column)++; return TOKEN_RBRACE;
        case '(': (*column)++; return TOKEN_LPAREN;
        case ')': (*column)++; return TOKEN_RPAREN;
        case ';': (*column)++; return TOKEN_SEMICOLON;
        case ',': (*column)++; return TOKEN_COMMA;
        case '+': (*column)++; return TOKEN_PLUS;
        case '=': (*column)++; return TOKEN_ASSIGN;
        case '"': (*column)++; return TOKEN_LIT_STRING;
        default: break;
    }

    if (isalpha(input[line][*column])) return TOKEN_IDENTIFIER;
    else if (isdigit(input[line][*column])) return TOKEN_LIT_INT;

    return TOKEN_UNKNOWN;
}

static bool slug_lexer_get_keyword(TokenList* list, char* input[], int line, int* column) {
    if (strncmp(&input[line][*column], "let", 3) == 0) {
        Token token = slug_lexer_create_token(TOKEN_KEYWORD_LET, line, *column, NULL, NULL, 0);
        slug_list_alloc_token(list, token);
        (*column) += 3;
        return true;
    }
    return false;
}

static void slug_lexer_get_identifier(TokenList* list, char* input[], int line, int* column) {
    if (isalpha(input[line][*column])) {
        size_t size = 0;
        char id[256] = {0};
        while (isalnum(input[line][*column])) {
            if (size >= 255) {
                printf("Identifier too long >= 255 characters\n");
                exit(1);
            }
            id[size++] = input[line][*column];
            (*column)++;
        }
        Token token = slug_lexer_create_token(TOKEN_IDENTIFIER, line, *column, id, NULL, 0);
        slug_list_alloc_token(list, token);
    }
}

static void slug_lexer_get_literal_int32(TokenList* list, char* input[], int line, int* column) {
    int value = 0;
    while (isdigit(input[line][*column])) {
        value = value * 10 + (input[line][*column] - '0');
        (*column)++;
    }
    Token token = slug_lexer_create_token(TOKEN_LIT_INT, line, *column, NULL, NULL, value);
    slug_list_alloc_token(list, token);
}

static void slug_lexer_get_literal_string(TokenList* list, char* input[], int line, int* column) {
    char lit_string[1024] = {0};
    int i = 0;
    while (input[line][*column] != '"' && input[line][*column] != '\0') {
        lit_string[i++] = input[line][*column];
        (*column)++;
    }
    (*column)++;
    Token token = slug_lexer_create_token(TOKEN_LIT_STRING, line, *column, NULL, lit_string, 0);
    slug_list_alloc_token(list, token);
}

TokenList* slug_lexer_tokenize(char* input[]) {
    TokenList* list = malloc(sizeof(TokenList));
    list->size = 0;
    list->capacity = 4;
    list->data = malloc(list->capacity * sizeof(Token));

    int line = 0;
    while (input[line] != NULL) {
        int column = 0;
        while (column < strlen(input[line])) {
            if (isspace(input[line][column])) {
                column++;
                continue;
            }
            if (slug_lexer_get_keyword(list, input, line, &column)) continue;

            TokenKind kind = slug_lexer_get_symbol(input, line, &column);
            switch (kind) {
                case TOKEN_IDENTIFIER:
                    slug_lexer_get_identifier(list, input, line, &column);
                    break;
                case TOKEN_LIT_INT:
                    slug_lexer_get_literal_int32(list, input, line, &column);
                    break;
                case TOKEN_LIT_STRING:
                    slug_lexer_get_literal_string(list, input, line, &column);
                    break;
                case TOKEN_UNKNOWN:
                    printf("Unknown token at line %d, column %d\n", line, column);
                    column++;
                    break;
                default:
                    Token token = slug_lexer_create_token(kind, line, column, NULL, NULL, 0);
                    slug_list_alloc_token(list, token);
                    break;
            }
        }
        line++;
    }
    Token token = slug_lexer_create_token(TOKEN_EOF, 0, 0, NULL, NULL, 0);
    slug_list_alloc_token(list, token);
    return list;
}

void slug_free_list(TokenList* list) {
    if (list == NULL) {
        printf("Error: Token list not allocated\n");
        exit(1);
    }
    free(list->data);
    free(list);
}
