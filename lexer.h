// lexer.h

#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

typedef enum {
    TOKEN_INTEGER,    // 整数値
    TOKEN_FLOAT,      // 浮動小数点数
    TOKEN_PLUS,       // +
    TOKEN_MINUS,      // -
    TOKEN_TIMES,      // *
    TOKEN_DIVIDE,     // /
    TOKEN_PRINT,      // print
    TOKEN_STRING,     // 文字列
    TOKEN_LPAREN,     // (
    TOKEN_RPAREN,     // )
    TOKEN_SEMICOLON,  // ;
    TOKEN_SYMBOL,     // "
    TOKEN_COMMENT,    // #
    TOKEN_NEWLINE,    // 改行
    TOKEN_COMMENTS,   // コメントアウトの内容
    TOKEN_VARIABLE,   // 変数名
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;    // トークンの種類
    char* lexeme;      // トークンの文字列
    long value;        // トークンの値 (整数や浮動小数点数の場合に使用)
} Token;

// 字句解析を行う関数
Token* tokenize(const char* code);

// トークン配列のメモリを解放する関数
void free_tokens(Token* tokens);
void print_tokens(const Token* tokens);

#endif
