#include <slug_lexer.h>
#include <slug_rw.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    SourceFile* file = slug_rw_read_file("tests/main.slug");
    if (file == NULL) {
        printf("Error: Could not open file\n");
        exit(1);
    }
    TokenList* tokens = slug_lexer_tokenize(file);

    for (size_t i = 0; i < tokens->size; i++) {
        Token token = tokens->data[i];
        printf("Token kind: %s, Line: %d, Column: %d, ", slug_kind_to_string(token), token.line, token.column);
        if (token.selfKind == TOKEN_IDENTIFIER) {
            printf("Identifier: %s\n", token.ident);
        } else if (token.selfKind == TOKEN_LIT_INT) { 
            printf("Literal int: %d\n", token.literal.lit_i32);
        } else if (token.selfKind == TOKEN_LIT_STRING) {
            printf("Literal string: %s\n", token.literal.lit_string);
        } else {
            printf("\n");
        }
    }

    slug_rw_free(file);
    slug_lexer_free_list(tokens);
    return 0;
}
