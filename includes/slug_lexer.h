#ifndef _SLUG_LEXER_H
#define _SLUG_LEXER_H
#include <stdint.h>
#include <stddef.h>

typedef struct Token Token;
typedef struct TokenList TokenList;


typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD_LET,
    TOKEN_LIT_INT,
    TOKEN_LIT_STRING,
    TOKEN_SEMICOLON,
    TOKEN_ASSIGN,
    TOKEN_COMMA,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_PLUS,  
    TOKEN_UNKNOWN, 
    TOKEN_EOF
} TokenKind;

struct TokenList {
    Token* data; 
    size_t size;
    size_t capacity;
};

struct Token {
    TokenKind selfKind;
    const char* ident;
    int line, column;
    union {
        int32_t lit_i32;
        char* lit_string;
    } literal;
};

TokenList* slug_lexer_tokenize(char* input[]);
void slug_free_list(TokenList* list);

#endif // _SLUG_LEXER_H 