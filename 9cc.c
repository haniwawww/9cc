#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
    TK_RESERVED, // Symbol
    TK_NUM, // Number token
    TK_EOF, // End of input token
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind; // Type of token
    Token *next; // Next input token
    int val; // Number if TK_NUM
    char *str; // String token
};

// Current target token
Token *token;

// Function to report error
// Take same arguments with printf
void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// If next token are expected token, go next token and return true
// If other, return false
bool consume(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
        return false;
    token = token->next;
    return true;
}

// If next token are expected token, go next token
// If other, report error
void expect(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
        error("It is not '%c'", op);
    token = token->next;
}

// If next token are number, go next token and return the number
// If other, report error
int expect_number() {
    if (token->kind != TK_NUM)
        error("Not the number");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *cur, char *str) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

// Return tokenized input p what is string
Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        // Skip void character
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-') {
            cur = new_token(TK_RESERVED, cur, p++);
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error("Could not be tokenized");
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Invalid counts of arguments\n");
        return 1;
    }

    // Tokenize
    token = tokenize(argv[1]);

    // Output front of assembly
    printf(".intel_syntax noprefix\n");
    printf(".globl _main\n");
    printf("_main:\n");

    // The first of formula must be the number.
    // So check it and output first "mov" order.
    printf("    mov rax, %d\n", expect_number());

    // If '+' or '-', consume token and output assembly
    while (!at_eof()) {
        if (consume('+')) {
            printf("    add rax, %d\n", expect_number());
            continue;
        }

        expect('-');
        printf("    sub rax, %d\n", expect_number());
    }

    printf("    ret\n");
    return 0;
}