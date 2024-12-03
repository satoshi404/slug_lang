#ifndef _SLUG_LEXER_H
#define _SLUG_LEXER_H
#include "slug_rw.h"
#include <stdint.h>
#include <stddef.h>

typedef struct Token Token;
typedef struct TokenList TokenList;


typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD_LET,
    TOKEN_KEYWORD_FUNCTION,
    TOKEN_KEYWORD_RETURN,
    TOKEN_LIT_INT,
    TOKEN_LIT_STRING,
    TOKEN_SEMICOLON,
    TOKEN_ASSIGN,
    TOKEN_COMMA,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_UNKNOWN,
    // --------------------------------
    // Operators
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_TIMES,
    TOKEN_DIVIDE,
    // --------------------------------
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

TokenList* slug_lexer_tokenize(SourceFile* file);
void slug_lexer_free_list(TokenList* list);

char* slug_kind_to_string(Token token);

#endif // _SLUG_LEXER_H 