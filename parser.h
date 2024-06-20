#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV
} BinaryOperator;

typedef enum {
    NODE_CONSTANT,         // 定数ノード
    NODE_BINARY_OP,        // 二項演算子ノード
    NODE_PRINT_STATEMENT,  // print文ノード
} NodeType;

typedef struct s_ast_node {
    NodeType type;          // ノードの種類
    long value;             // 定数の値
    char* str_value;        // 文字列定数の値
    struct s_ast_node* left;
    struct s_ast_node* right;
    BinaryOperator op;      // 二項演算子の種類を示すフィールドを追加
} ASTNode;

ASTNode* parse(Token* t);
void evaluate(ASTNode* node);
void free_ast(ASTNode* node);
void evaluate_print_statement(ASTNode* node);
long evaluate_expression(ASTNode* node);

void print_ast(ASTNode* node, int depth);
const char* node_type_to_string(NodeType type);

#endif