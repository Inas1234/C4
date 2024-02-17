#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Tokenizer.h"

typedef struct {
    char * value;
} NodeExprIdent;

typedef struct {
    int value;
} NodeExprNumber;

typedef enum {
    NODE_EXPR_IDENT,
    NODE_EXPR_NUMB
} NodeExprType;

typedef union {
    NodeExprIdent ident;
    NodeExprNumber numb;
} NodeExprData;

typedef struct {
    NodeExprType type;
    NodeExprData data;
} NodeExpr;

typedef struct {
    NodeExpr code;
} NodeStmtExit;

typedef struct 
{
    NodeExpr data;
} NodeStmtPrintln;


typedef enum {
    NODE_STMT_EXIT,
    NODE_STMT_PRINTLN
} NodeStmtType;

typedef union {
    NodeStmtExit exit_in;
    NodeStmtPrintln printf_in;
} NodeStmtData;

typedef struct {
    NodeStmtType type;
    NodeStmtData data;
} NodeStmt;


typedef struct {
    NodeStmt* smts;
    int length;
} NodeProg;

static int indexP =0;


void printExpr(NodeExpr expr) {
    switch (expr.type) {
        case NODE_EXPR_IDENT:
            printf("Identifier: %s\n", expr.data.ident.value);
            break;
        case NODE_EXPR_NUMB:
            printf("Number: %d\n", expr.data.numb.value);
            break;
        default:
            printf("Unknown expression type\n");
    }
}

void printStmt(NodeStmt stmt) {
    switch (stmt.type) {
        case NODE_STMT_EXIT:
            printf("Exit Statement: \n");
            printExpr(stmt.data.exit_in.code);
            break;
        case NODE_STMT_PRINTLN:
            printf("Printf Statement: \n");
            printExpr(stmt.data.printf_in.data);
            break;
        default:
            printf("Unknown statement type\n");
    }
}

void printNodeProg(NodeProg prog) {
    printf("Program with %d statements:\n", prog.length);
    for (int i = 0; i < prog.length; ++i) {
        printf("Statement %d:\n", i + 1);
        printStmt(prog.smts[i]);
    }
}


Token peekP(Token* tokens, int token_length, int ahead){
    if (ahead + indexP >= token_length){
        return (Token){END, NULL};
    }
    return tokens[indexP + ahead];
}


Token consumeP(Token* tokens){
    return tokens[indexP++];
}


NodeExpr parse_expr(Token* tokens, int token_length){
    NodeExpr node;
    if (peekP(tokens, token_length, 0).type == NUMBER){
        node.type = NODE_EXPR_NUMB;
        node.data.numb.value = atoi(consumeP(tokens).value);
        return node;
    }
    else if(peekP(tokens, token_length, 0).type == IDENTIFIER){
        node.type = NODE_EXPR_IDENT;
        node.data.ident.value = consumeP(tokens).value;
        return node;
    } 
}


NodeStmt parse_stmt(Token* tokens, int token_length){
    NodeStmt node;
    if (peekP(tokens, token_length, 0).type == EXIT){
        consumeP(tokens);
        if (!peekP(tokens, token_length, 0).type == OPEN_PAREN){
            printf("Missing open paren in exit stmt on line %d\n", peekP(tokens, token_length, 0).line);
        }
        consumeP(tokens);
        NodeExpr expr = parse_expr(tokens, token_length);

        if (peekP(tokens, token_length, 0).type != CLOSE_PAREN){
            printf("Missing close paren after exit stmt expression on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }
        consumeP(tokens);


        if (peekP(tokens, token_length, 0).type != SEMICOLON){
            printf("Missing semi colon after exit on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }
        consumeP(tokens);

        node.type = NODE_STMT_EXIT;
        node.data.exit_in.code = expr;
    }
    else if (peekP(tokens, token_length, 0).type == PRINTLN){
        consumeP(tokens);
        if (!peekP(tokens, token_length, 0).type == OPEN_PAREN){
            printf("Missing open paren in printf stmt on line %d\n", peekP(tokens, token_length, 0).line);
        }
        consumeP(tokens);
        NodeExpr expr = parse_expr(tokens, token_length);

        if (peekP(tokens, token_length, 0).type != CLOSE_PAREN){
            printf("Missing close paren after printf stmt expression on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }
        consumeP(tokens);


        if (peekP(tokens, token_length, 0).type != SEMICOLON){
            printf("Missing semi colon after printf on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }
        consumeP(tokens);

        node.type = NODE_STMT_PRINTLN;
        node.data.printf_in.data = expr;
    }
    
    return node;
}

NodeProg parseProg(Token* tokens, int token_length) {
    int capacity = 10; 
    NodeStmt* statements = (NodeStmt*)malloc(capacity * sizeof(NodeStmt));
    if (!statements) {
        printf("Failed to allocate memory for statements\n");
        exit(1);
    }

    int length = 0; 

    while (indexP < token_length) {
        if (peekP(tokens, token_length, 0).type == EXIT || peekP(tokens, token_length, 0).type == PRINTLN) {
            if (length >= capacity) {
                capacity *= 2;
                statements = (NodeStmt*)realloc(statements, capacity * sizeof(NodeStmt));
                if (!statements) {
                    printf("Failed to reallocate memory for statements\n");
                    exit(1);
                }
            }

            statements[length++] = parse_stmt(tokens, token_length);
        } else {
            indexP++;
        }
    }

    NodeProg prog = {statements, length};
    return prog;
}
