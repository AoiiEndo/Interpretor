// lexer.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

// トークン配列の初期サイズ
#define INITIAL_TOKEN_SIZE 100

// トークンのサイズ
#define TOKEN_SIZE 256

// トークン配列を出力する関数
void print_tokens(const Token* tokens) {
    const char* token_type_names[] = {
        "TOKEN_INTEGER",    // 整数値
        "TOKEN_FLOAT",      // 浮動小数点数
        "TOKEN_PLUS",       // +
        "TOKEN_MINUS",      // -
        "TOKEN_TIMES",      // *
        "TOKEN_DIVIDE",     // /
        "TOKEN_PRINT",      // print
        "TOKEN_STRING",     // 文字列
        "TOKEN_LPAREN",     // (
        "TOKEN_RPAREN",     // )
        "TOKEN_SEMICOLON",  // ;
        "TOKEN_SYMBOL",     // "
        "TOKEN_COMMENT",    // #
        "TOKEN_NEWLINE",    // 改行
        "TOKEN_COMMENTS",   // コメントアウトの内容
        "TOKEN_VARIABLE",   // 変数名
    };

    for (int i = 0; i < 23; i++) {
        printf("Token Type: %s, Lexeme: %s, Value: %ld\n", 
               token_type_names[tokens[i].type], 
               tokens[i].lexeme ? tokens[i].lexeme : "NULL", 
               tokens[i].value);
    }
}

// 字句解析を行う関数
Token* tokenize(const char* code) {
    int code_length = strlen(code);
    Token* tokens = (Token*)malloc(INITIAL_TOKEN_SIZE * sizeof(Token));
    int tokens_size = INITIAL_TOKEN_SIZE;
    int tokens_index = 0;

    int i = 0;
    // printf("length: %d\n", code_length);
    while (i < code_length) {
        // printf("code: %c\n", code[i]);
        // printf("num: %d\n", i);
        // 空白類文字をスキップ
        if (isspace(code[i])) {
            if (code[i] == '\n') {
                tokens[tokens_index].type = TOKEN_NEWLINE;
                tokens[tokens_index].lexeme = "改行";
                tokens_index++;
            }
            i++;
            continue;
        }

        // 整数または浮動小数点数の解析
        if (isdigit(code[i])) {
            char token[TOKEN_SIZE];
            int token_index = 0;

            while (isdigit(code[i])) {
                token[token_index++] = code[i++];
            }

            if (code[i] == '.') {
                token[token_index++] = code[i++];

                while (isdigit(code[i])) {
                    token[token_index++] = code[i++];
                }

                token[token_index] = '\0';
                tokens[tokens_index].type = TOKEN_FLOAT;
                tokens[tokens_index].value = atof(token); //文字列をlong型に変換
            } else {
                token[token_index] = '\0';
                tokens[tokens_index].type = TOKEN_INTEGER;
                tokens[tokens_index].value = atol(token);
            }

            tokens[tokens_index].lexeme = strdup(token); //ストレージの予約(解放必須)
            tokens_index++;

            if (tokens_index >= tokens_size) {
                tokens_size *= 2;
                tokens = (Token*)realloc(tokens, tokens_size * sizeof(Token));
                if (!tokens) {
                    perror("Memory allocation failed");
                    exit(1);
                }
            }

            continue;
        }

        // 他のトークンの解析
        switch (code[i]) {
            case '+':
                tokens[tokens_index].type = TOKEN_PLUS;
                tokens[tokens_index].lexeme = "+";
                tokens_index++;
                i++;
                break;
            case '-':
                tokens[tokens_index].type = TOKEN_MINUS;
                tokens[tokens_index].lexeme = "-";
                tokens_index++;
                i++;
                break;
            case '*':
                tokens[tokens_index].type = TOKEN_TIMES;
                tokens[tokens_index].lexeme = "*";
                tokens_index++;
                i++;
                break;
            case '/':
                tokens[tokens_index].type = TOKEN_DIVIDE;
                tokens[tokens_index].lexeme = "/";
                tokens_index++;
                i++;
                break;
            case '(':
                tokens[tokens_index].type = TOKEN_LPAREN;
                tokens[tokens_index].lexeme = "(";
                tokens_index++;
                i++;
                break;
            case ')':
                tokens[tokens_index].type = TOKEN_RPAREN;
                tokens[tokens_index].lexeme = ")";
                tokens_index++;
                i++;
                break;
            case '"':
                tokens[tokens_index].type = TOKEN_SYMBOL;
                tokens[tokens_index].lexeme = "\"";
                tokens_index++;
                i++;
                int token_index = 0;
                char str_token[TOKEN_SIZE];

                while (code[i] != '"') {
                    str_token[token_index++] = code[i++];
                }

                str_token[token_index] = '\0';
                tokens[tokens_index].type = TOKEN_STRING;
                tokens[tokens_index].value = atof(str_token);
                tokens[tokens_index].lexeme = strdup(str_token);
                tokens_index++;

                tokens[tokens_index].type = TOKEN_SYMBOL;
                tokens[tokens_index].lexeme = "\"";
                tokens_index++;
                i++; //締めの"分足す
                break;
            case '#':
                tokens[tokens_index].type = TOKEN_COMMENT;
                tokens[tokens_index].lexeme = "#";
                tokens_index++;
                i++;
                break;
            case ';':
                tokens[tokens_index].type = TOKEN_SEMICOLON;
                tokens[tokens_index].lexeme = ";";
                tokens_index++;
                i++;
                break;
            default:{
                if (isalpha(code[i])) {
                    char token[TOKEN_SIZE];
                    int token_index = 0;

                    while (isalnum(code[i]) || code[i] == '_') {
                        token[token_index++] = code[i++];
                    }

                    token[token_index] = '\0';

                    if (strcmp(token, "print") == 0) {
                        tokens[tokens_index].type = TOKEN_PRINT;
                    } else {
                        tokens[tokens_index].type = TOKEN_VARIABLE;
                    }

                    tokens[tokens_index].lexeme = strdup(token);
                    tokens_index++;

                    if (tokens_index >= tokens_size) {
                        tokens_size *= 2;
                        tokens = (Token*)realloc(tokens, tokens_size * sizeof(Token));
                        if (!tokens) {
                            perror("Memory allocation failed");
                            exit(1);
                        }
                    }
                } else {
                    fprintf(stderr, "Unexpected character: %c\n", code[i]);
                    exit(1);
                }
                break;
            }
        }
    }
    return tokens;
}

// トークン配列のメモリを解放する関数
void free_tokens(Token* tokens) {
    for (int i = 0; tokens[i].lexeme != NULL; i++) {
        free(tokens[i].lexeme);
    }
    free(tokens);
}