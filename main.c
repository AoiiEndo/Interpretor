// main.c

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

#define MAX_CODE_SIZE 1000

// ファイルを読み込む関数
char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        perror("Memory allocation failed");
        exit(1);
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';

    fclose(file);

    return buffer;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // ファイルを読み込む
    char* code = read_file(argv[1]);

    // 字句解析を行う
    Token* tokens = tokenize(code);

    // 字句解析結果を出力する（デバッグ用）
    // print_tokens(tokens);

    // 構文解析を行う
    ASTNode* root = parse(tokens);
    // 構文解析結果を出力する（デバッグ用）
    // print_ast(root, 0);

    // ASTを評価する
    evaluate(root);

    // メモリの解放
    free_ast(root);
    // free_tokens(tokens);
    free(code);
    free(tokens);

    return 0;
}
