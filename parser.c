#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

static int current_token_index = 0; // トークン配列の現在のインデックス
static Token* tokens; // トークン配列

// ASTノードのタイプを文字列に変換する関数
const char* node_type_to_string(NodeType type) {
    switch (type) {
        case NODE_PRINT_STATEMENT: return "NODE_PRINT_STATEMENT";
        case NODE_CONSTANT: return "NODE_CONSTANT";
        case NODE_BINARY_OP: return "NODE_BINARY_OP";
        default: return "UNKNOWN";
    }
}

void print_ast(ASTNode* node, int depth) {
    if (node == NULL) return;

    // インデントを表示
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("%s", node_type_to_string(node->type)); // ノードタイプを表示

    // ノードの詳細を表示
    if (node->type == NODE_CONSTANT) {
        if (node->str_value != NULL) {
            printf(" (str_value: %s)", node->str_value);
        } else {
            printf(" (value: %ld)", node->value);
        }
    }

    printf("\n");

    // 左右の子ノードを再帰的に表示
    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
}

// 新しいASTノードを作成する関数
static ASTNode* new_node(NodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        perror("Memory allocation failed");
        exit(1);
    }
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->str_value = NULL;
    return node;
}

// 二項演算子を持つASTノードを作成する関数
static ASTNode* new_binary_op_node(ASTNode* left, ASTNode* right, BinaryOperator op) {
    ASTNode* node = new_node(NODE_BINARY_OP);
    node->left = left;
    node->right = right;
    node->op = op; // 演算子を設定
    return node;
}

// 整数定数を持つASTノードを作成する関数
static ASTNode* new_constant_node(long value) {
    ASTNode* node = new_node(NODE_CONSTANT);
    node->value = value;
    return node;
}

// 文字列定数を持つASTノードを作成する関数
static ASTNode* new_string_node(char* str_value) {
    ASTNode* node = new_node(NODE_CONSTANT);
    node->str_value = strdup(str_value);
    return node;
}

static ASTNode* parse_print_statement(); // print文を解析する関数のプロトタイプ宣言
static ASTNode* parse_expression(); // 式を解析する関数 (static 修飾子を追加)
static ASTNode* parse_term(); // 項を解析する関数
static ASTNode* parse_factor(); // 因子を解析する関数

// ASTノードを解放する関数
void free_ast(ASTNode* node) {
    if (node == NULL) return;

    if (node->left != NULL) {
        free_ast(node->left);
        node->left = NULL;
    }
    if (node->right != NULL) {
        free_ast(node->right);
        node->right = NULL;
    }
    free(node->str_value);
    free(node);
}

// 字句解析と構文解析を行う関数
ASTNode* parse(Token* t) {
    tokens = t;
    current_token_index = 0; // インデックスをリセット
    return parse_print_statement();
}

// ASTを評価する関数
void evaluate(ASTNode* node) {
    if (node == NULL) return;

    switch (node->type) {
        case NODE_PRINT_STATEMENT:
            evaluate_print_statement(node);
            break;
        case NODE_CONSTANT: // 定数ノードの場合は何もしない
            break;
        case NODE_BINARY_OP: // 二項演算子ノードの場合は何もしない
            break;
        default:
            fprintf(stderr, "Unknown AST node type\n");
            exit(1);
    }
}

// print文を評価する関数
void evaluate_print_statement(ASTNode* node) {
    ASTNode* expr = node->left;

    if (expr->type == NODE_CONSTANT) {
        if (expr->str_value != NULL) {
            printf("%s\n", expr->str_value);
        } else {
            printf("%ld\n", expr->value);
        }
    } else if (expr->type == NODE_BINARY_OP) {
        long result = evaluate_expression(expr);
        printf("%ld\n", result);
    } else {
        fprintf(stderr, "Invalid expression in print statement\n");
        exit(1);
    }

    if (node->right != NULL) {
        evaluate_print_statement(node->right);
    }
}

// 式を評価する関数 (再帰的に呼ばれる)
long evaluate_expression(ASTNode* node) {
    if (node->type == NODE_CONSTANT) {
        return node->value;
    } else if (node->type == NODE_BINARY_OP) {
        long left_val = evaluate_expression(node->left);
        long right_val = evaluate_expression(node->right);

        BinaryOperator op = node->op; // 初期化

        switch (op) {
            case OP_ADD:
                return left_val + right_val;
            case OP_SUB:
                return left_val - right_val;
            case OP_MUL:
                return left_val * right_val;
            case OP_DIV:
                if (right_val == 0) {
                    fprintf(stderr, "Division by zero error\n");
                    exit(1);
                }
                return left_val / right_val;
            default:
                fprintf(stderr, "Unknown operator in expression\n");
                exit(1);
        }
    } else {
        fprintf(stderr, "Invalid node type in expression\n");
        exit(1);
    }
}

// print文を解析する関数
static ASTNode* parse_print_statement() {
    ASTNode* stmt = NULL;
    ASTNode* current_stmt = NULL;
    
    while (tokens[current_token_index].type == TOKEN_PRINT) {
        current_token_index++; // print分足す

        // 次が(のトークンではないならエラー
        if (tokens[current_token_index].type != TOKEN_LPAREN) {
            fprintf(stderr, "Expected '(' after 'print'\n");
            exit(1);
        }
        current_token_index++; //(分足す

        ASTNode* expr = NULL;
        if (tokens[current_token_index].type == TOKEN_SYMBOL) { // 文字列なら
            current_token_index++;
            if (tokens[current_token_index].type == TOKEN_STRING && tokens[++current_token_index].type == TOKEN_SYMBOL) {
                expr = new_string_node(tokens[current_token_index - 1].lexeme);
                current_token_index++;
            } else {
                fprintf(stderr, "Expected '\"' after expression\n");
                exit(1);
            }
        } else if (tokens[current_token_index].type == TOKEN_INTEGER || tokens[current_token_index].type == TOKEN_FLOAT) { // 式の場合
            expr = parse_expression();
        } else { // それ以外
            fprintf(stderr, "Unexpected token\n");
            exit(1);
        }

        // )が来ているか
        if (tokens[current_token_index].type != TOKEN_RPAREN) {
            fprintf(stderr, "Expected ')' after expression\n");
            exit(1);
        }
        current_token_index++;
        // ;が来ているか
        if (tokens[current_token_index].type != TOKEN_SEMICOLON) {
            fprintf(stderr, "Expected ')' after expression\n");
            exit(1);
        }
        current_token_index++;
        if (tokens[current_token_index].type == TOKEN_NEWLINE) {
            current_token_index++;
        }

        if (stmt == NULL) {
            stmt = new_node(NODE_PRINT_STATEMENT);
            stmt->left = expr;
            current_stmt = stmt;
        } else {
            current_stmt->right = new_node(NODE_PRINT_STATEMENT);
            current_stmt = current_stmt->right;
            current_stmt->left = expr;
        }
    }
    return stmt;
}

// 式を解析する関数
static ASTNode* parse_expression() {
    ASTNode* left = parse_term();

    while (tokens[current_token_index].type == TOKEN_PLUS ||
           tokens[current_token_index].type == TOKEN_MINUS) {
        BinaryOperator op = -1;
        if (tokens[current_token_index].type == TOKEN_PLUS) {
            op = OP_ADD;
        } else if (tokens[current_token_index].type == TOKEN_MINUS) {
            op = OP_SUB;
        }
        current_token_index++; // '+' または '-' トークンを読み飛ばす
        ASTNode* right = parse_term();
        left = new_binary_op_node(left, right, op);
    }
    return left;
}

// 項を解析する関数
static ASTNode* parse_term() {
    ASTNode* left = parse_factor();

    while (tokens[current_token_index].type == TOKEN_TIMES ||
           tokens[current_token_index].type == TOKEN_DIVIDE) {
        BinaryOperator op = -1;
        if (tokens[current_token_index].type == TOKEN_TIMES) {
            op = OP_MUL;
        } else if (tokens[current_token_index].type == TOKEN_DIVIDE) {
            op = OP_DIV;
        }
        current_token_index++; // '*' または '/' トークンを読み飛ばす
        ASTNode* right = parse_factor();
        left = new_binary_op_node(left, right, op);
    }
    return left;
}

// 因子を解析する関数
static ASTNode* parse_factor() {
    ASTNode* factor = NULL;

    if (tokens[current_token_index].type == TOKEN_INTEGER) {
        factor = new_constant_node(tokens[current_token_index].value);
        current_token_index++;
    } else if (tokens[current_token_index].type == TOKEN_LPAREN) {
        current_token_index++;
        factor = parse_expression();
        if (tokens[current_token_index].type != TOKEN_RPAREN) {
            fprintf(stderr, "Expected ')' after expression\n");
            exit(1);
        }
        current_token_index++;
    } else {
        fprintf(stderr, "Unexpected token\n");
        exit(1);
    }
    return factor;
}
