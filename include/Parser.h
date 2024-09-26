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
    NODE_EXPR_NUMB,
    NODE_EXPR_BINARY,
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
    Token token;
    NodeExpr* left;
    NodeExpr* right;
} BinaryExpressionPlus;


typedef struct {
    NodeExpr code;
} NodeStmtExit;


typedef struct{
    NodeExpr name;
    NodeExpr *args;
    int argCount;
}NodeStmtFuncCall;

typedef struct 
{
    NodeExpr data;
    NodeStmtFuncCall *func_call;
} NodeStmtPrintln;


typedef struct {
    NodeExpr value;
} NodeStmtReturn;

typedef struct {
    NodeExpr left;
    NodeExpr right;
} NodeStmtAssign;

typedef struct {
    NodeExpr name;
    char * type;
    NodeExpr * optionalInit;
} NodeStmtDeclare;

typedef enum {
    NODE_STMT_EXIT,
    NODE_STMT_PRINTLN,
    NODE_STMT_FUNCALL,
    NODE_STMT_RETURN,
    NODE_STMT_ASSIGN,
    NODE_STMT_IF,
    NODE_STMT_WHILE,
    NODE_STMT_DECLARE,
} NodeStmtType;

typedef struct NodeStmt NodeStmt; 

typedef struct {
    NodeExpr condition;
    NodeStmt* ifStmts;
    int thenCount;
    NodeStmt* elseStmts;
    int elseCount;  
} NodeStmtIF;

typedef struct {
    NodeExpr condition;
    NodeStmt* stmts;
    int stmt_count;
} NodeStmtWhile;

typedef union {
    NodeStmtExit exit_in;
    NodeStmtPrintln printf_in;
    NodeStmtFuncCall func_call;
    NodeStmtReturn return_in;
    NodeStmtAssign assign_in;
    NodeStmtIF if_in;
    NodeStmtWhile while_in;
    NodeStmtDeclare declare_in;
} NodeStmtData;

struct NodeStmt {
    NodeStmtType type;
    NodeStmtData data;
};

typedef struct {
    NodeExpr name;
    char * type;
} NodeFuncArguments;

typedef enum {
    NODE_FUNC_INT,
    NODE_FUNC_VOID,
} ReturnType;

typedef struct {
    ReturnType returnType;
    NodeFuncArguments * args;
    NodeExpr name;
    NodeStmt* stmts;
    int arg_count;
    int stmt_count;
    NodeExpr returnValue;
} NodeFunc;


typedef struct {
    NodeFunc * functions;
    NodeStmt* smts;
    int length;
    int fun_length;
} NodeProg;

static int indexP =0;


void printExpr(NodeExpr expr) {
    switch (expr.type) {
        case NODE_EXPR_IDENT:
            printf("-Identifier: %s\n", expr.data.ident.value);
            break;
        case NODE_EXPR_NUMB:
            printf("-Number: %d\n", expr.data.numb.value);
            break;
        case NODE_EXPR_BINARY:
            printf("-Binary expression: \n");
            BinaryExpressionPlus* binaryExpr = (BinaryExpressionPlus*)expr.data.ident.value;
            printf("Left: \n");
            printExpr(*binaryExpr->left);
            printf("Right: \n");
            printExpr(*binaryExpr->right);
            break;
        default:
            printf("Unknown expression type\n");
    }
}

void printFuncCall(NodeStmtFuncCall* func_call) {
    printf("-Function call: \n");
    printExpr(func_call->name);
    printf("-Args: \n");
    for (int i = 0; i < func_call->argCount; i++)
    {
        printExpr(func_call->args[i]);
    }
}

void printStmt(NodeStmt stmt) {
    switch (stmt.type) {
        case NODE_STMT_EXIT:
            printf("=Exit Statement: \n");
            printExpr(stmt.data.exit_in.code);
            break;
        case NODE_STMT_PRINTLN:
            printf("=Printf Statement: \n");
            if (stmt.data.printf_in.func_call != NULL) {
                printFuncCall(stmt.data.printf_in.func_call);
            } else {
                printExpr(stmt.data.printf_in.data);
            }
            break;
        case NODE_STMT_FUNCALL:
            printf("=Funcion call Statement: \n");
            printExpr(stmt.data.func_call.name);
            printf("-Args: \n");
            for (int i = 0; i < stmt.data.func_call.argCount; i++)
            {
                printExpr(stmt.data.func_call.args[i]);
            }
            break;
        case NODE_STMT_RETURN:
            printf("=Return Statement: \n");
            printExpr(stmt.data.return_in.value);
            break;
        case NODE_STMT_ASSIGN:
            printf("=Assignment Statement: \n");
            printExpr(stmt.data.assign_in.left);
            printExpr(stmt.data.assign_in.right);
            break;
        case NODE_STMT_IF:
            printf("=If Statement: \n");
            printExpr(stmt.data.if_in.condition);
            printf("Then:\n");
            for (int i = 0; i < stmt.data.if_in.thenCount; i++)
            {
                printStmt(stmt.data.if_in.ifStmts[i]);
            }
            printf("Else:\n");
            for (int i = 0; i < stmt.data.if_in.elseCount; i++)
            {
                printStmt(stmt.data.if_in.elseStmts[i]);
            }
            break;
        case NODE_STMT_WHILE:
            printf("=While Statement: \n");
            printExpr(stmt.data.while_in.condition);
            for (int i = 0; i < stmt.data.while_in.stmt_count; i++)
            {
                printStmt(stmt.data.while_in.stmts[i]);
            }
            break;
        case NODE_STMT_DECLARE:
            printf("=Declare Statement: \n");
            printExpr(stmt.data.declare_in.name);
            printf("Type: %s\n", stmt.data.declare_in.type);
            if (stmt.data.declare_in.optionalInit != NULL) {
                printf("Init: \n");
                printExpr(*stmt.data.declare_in.optionalInit);
            }
            break;
        default:
            printf("Unknown statement type\n");
    }
}


void printNodeProg(NodeProg prog) {
    printf("Program with %d statements and %d functions:\n", prog.length, prog.fun_length);

    printf("\nStatements:\n");
    for (int i = 0; i < prog.length; ++i) {
        printf("\tStatement %d:\n", i + 1);
        printStmt(prog.smts[i]);
    }

    printf("\nFunctions:\n");
    for (int i = 0; i < prog.fun_length; ++i) {
        NodeFunc func = prog.functions[i];
        printf("Function %d: ", i + 1);
        printExpr(func.name); 
        printf("Arguments (%d):\n", func.arg_count);
        for (int j = 0; j < func.arg_count; ++j) {
            printf("\tArgument %d: ", j + 1);
            printExpr(func.args[j].name);
            printf("\tType: %s\n", func.args[j].type);
        }

        printf("Return type: %s\n", func.returnType == NODE_FUNC_INT ? "int" : "void");

        printf("Body (%d statements):\n", func.stmt_count);
        for (int k = 0; k < func.stmt_count; ++k) {
            printf("Statement %d:\n", k + 1);
            printStmt(func.stmts[k]);
        }
        printf("\n");
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

NodeExpr parse_expr(Token* tokens, int token_length);  


NodeExpr parse_binary_expr(Token* tokens, int token_length) {
    NodeExpr left = parse_expr(tokens, token_length);

    Token nextToken = peekP(tokens, token_length, 0);

    while (nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == MULTIPLY || nextToken.type == DIVIDE
        || nextToken.type == LESS_THAN || nextToken.type == GREATER_THAN || nextToken.type == LESS_THAN_EQUAL || 
        nextToken.type == GREATER_THAN_EQUAL || nextToken.type == EQUAL_EQUAL || nextToken.type == NOT_EQUAL) {
        Token operatorToken = consumeP(tokens);

        NodeExpr right = parse_expr(tokens, token_length);

        NodeExpr newExpr;
        newExpr.type = NODE_EXPR_IDENT; 
        BinaryExpressionPlus* binaryExpr = malloc(sizeof(BinaryExpressionPlus));
        if (!binaryExpr) {
            printf("Failed to allocate memory for binary expression\n");
            exit(1);
        }

        binaryExpr->token = operatorToken;
        binaryExpr->left = malloc(sizeof(NodeExpr));
        binaryExpr->right = malloc(sizeof(NodeExpr));
        if (!binaryExpr->left || !binaryExpr->right) {
            printf("Failed to allocate memory for binary expression nodes\n");
            exit(1);
        }
        *binaryExpr->left = left;
        *binaryExpr->right = right;

        newExpr.data.ident.value = (char*)binaryExpr;

        left = newExpr;

        nextToken = peekP(tokens, token_length, 0);
    }

    return left;
}



NodeExpr parse_expr(Token* tokens, int token_length) {
    // Parse the left-hand side (lhs) of the expression
    NodeExpr left;
    if (peekP(tokens, token_length, 0).type == NUMBER) {
        left.type = NODE_EXPR_NUMB;
        left.data.numb.value = atoi(consumeP(tokens).value);
    } else if (peekP(tokens, token_length, 0).type == IDENTIFIER) {
        left.type = NODE_EXPR_IDENT;
        left.data.ident.value = consumeP(tokens).value;
    } else {
        printf("Unexpected token type in expression on line %d\n", peekP(tokens, token_length, 0).line);
        exit(1);
    }

    Token nextToken = peekP(tokens, token_length, 0);
    while (nextToken.type == PLUS || nextToken.type == MINUS || nextToken.type == MULTIPLY || nextToken.type == DIVIDE
        || nextToken.type == LESS_THAN || nextToken.type == GREATER_THAN || nextToken.type == LESS_THAN_EQUAL ||
        nextToken.type == GREATER_THAN_EQUAL || nextToken.type == EQUAL_EQUAL || nextToken.type == NOT_EQUAL) {
        Token operatorToken = consumeP(tokens);  

        NodeExpr right;
        if (peekP(tokens, token_length, 0).type == NUMBER) {
            right.type = NODE_EXPR_NUMB;
            right.data.numb.value = atoi(consumeP(tokens).value);
        } else if (peekP(tokens, token_length, 0).type == IDENTIFIER) {
            right.type = NODE_EXPR_IDENT;
            right.data.ident.value = consumeP(tokens).value;
        } else {
            printf("Unexpected token type in expression on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1);
        }

        NodeExpr newExpr;
        newExpr.type = NODE_EXPR_BINARY; 

        BinaryExpressionPlus* binaryExpr = malloc(sizeof(BinaryExpressionPlus));
        if (!binaryExpr) {
            printf("Failed to allocate memory for binary expression\n");
            exit(1);
        }

        binaryExpr->token = operatorToken;
        binaryExpr->left = malloc(sizeof(NodeExpr));
        binaryExpr->right = malloc(sizeof(NodeExpr));
        if (!binaryExpr->left || !binaryExpr->right) {
            printf("Failed to allocate memory for binary expression nodes\n");
            exit(1);
        }

        *binaryExpr->left = left;
        *binaryExpr->right = right;

        newExpr.data.ident.value = (char*)binaryExpr;

        left = newExpr;

        // Check for more operators
        nextToken = peekP(tokens, token_length, 0);
    }

    return left;  
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

        if (peekP(tokens, token_length, 0).type == IDENTIFIER && peekP(tokens, token_length, 1).type == OPEN_PAREN){
            // Handle function call inside print statement
            node.type = NODE_STMT_PRINTLN;  // The overall statement type is still print
            node.data.printf_in.func_call = malloc(sizeof(NodeStmtFuncCall));
            node.data.printf_in.func_call->name = parse_expr(tokens, token_length);  // Parse function name
            consumeP(tokens);  // Consume '(' after function name
            
            int argsCapacity = 4;  // Initial capacity for function arguments
            node.data.printf_in.func_call->args = malloc(sizeof(NodeExpr) * argsCapacity);
            node.data.printf_in.func_call->argCount = 0;

            while (peekP(tokens, token_length, 0).type != CLOSE_PAREN) {
                if (node.data.printf_in.func_call->argCount >= argsCapacity) {
                    argsCapacity *= 2; 
                    node.data.printf_in.func_call->args = realloc(node.data.printf_in.func_call->args, sizeof(NodeExpr) * argsCapacity);
                }

                node.data.printf_in.func_call->args[node.data.printf_in.func_call->argCount++] = parse_expr(tokens, token_length);

                if (peekP(tokens, token_length, 0).type == COMMA) {
                    consumeP(tokens);  // Consume ',' between arguments
                }
            }
            consumeP(tokens);  // Consume ')'
            node.data.printf_in.data.type = NODE_EXPR_IDENT; 
            
            if (peekP(tokens, token_length, 0).type != CLOSE_PAREN){
                printf("Missing close paren after printf stmt expression on line %d\n", peekP(tokens, token_length, 0).line);
                exit(1);
            }
            consumeP(tokens);
        }
        else{
            node.data.printf_in.func_call = NULL;
            // Handle regular expression inside print statement
            NodeExpr expr = parse_expr(tokens, token_length);

            if (peekP(tokens, token_length, 0).type != CLOSE_PAREN){
                printf("Missing close paren after printf stmt expression on line %d\n", peekP(tokens, token_length, 0).line);
                exit(1);
            }
            consumeP(tokens);  // Consume ')'

            node.type = NODE_STMT_PRINTLN;
            node.data.printf_in.data = expr;
        }

        if (peekP(tokens, token_length, 0).type != SEMICOLON){
            printf("Missing semi colon after printf on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }
        consumeP(tokens);

    }
    else if (peekP(tokens, token_length, 0).type == IDENTIFIER && peekP(tokens, token_length, 1).type == OPEN_PAREN){
        node.type = NODE_STMT_FUNCALL;
        node.data.func_call.name = parse_expr(tokens, token_length);
        consumeP(tokens);
        int argsCapacity = 4; // Initial capacity
        node.data.func_call.args = malloc(sizeof(NodeExpr) * argsCapacity);
        node.data.func_call.argCount = 0;

        while (peekP(tokens, token_length, 0).type != CLOSE_PAREN) {
            if (node.data.func_call.argCount >= argsCapacity) {
                argsCapacity *= 2; 
                node.data.func_call.args = realloc(node.data.func_call.args, sizeof(NodeExpr) * argsCapacity);
            }

            node.data.func_call.args[node.data.func_call.argCount++] = parse_expr(tokens, token_length);

            if (peekP(tokens, token_length, 0).type == COMMA) {
                consumeP(tokens); 
            }
        }
        consumeP(tokens);
        if (peekP(tokens, token_length, 0).type != SEMICOLON) {
            printf("Missing semicolon after function call on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1);
        }
        consumeP(tokens);
        
    }
    else if (peekP(tokens, token_length, 0).type == RETURN){
        consumeP(tokens);
        NodeExpr expr = parse_expr(tokens, token_length);
        if (peekP(tokens, token_length, 0).type != SEMICOLON){
            printf("Missing semi colon after return on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }
        consumeP(tokens);
        node.type = NODE_STMT_RETURN;
        node.data.return_in.value = expr;
    }
    else if (peekP(tokens, token_length, 0).type == IDENTIFIER && peekP(tokens, token_length, 1).type == EQUAL){
        node.type = NODE_STMT_ASSIGN;
        node.data.assign_in.left = parse_expr(tokens, token_length);
        consumeP(tokens);
        node.data.assign_in.right = parse_expr(tokens, token_length);
        if (peekP(tokens, token_length, 0).type != SEMICOLON){
            printf("Missing semi colon after assignment on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }
        consumeP(tokens);
    }
    else if (peekP(tokens, token_length, 0).type == IF){
        consumeP(tokens);
        if (peekP(tokens, token_length, 0).type != OPEN_PAREN){
            printf("Missing open paren after if on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }
        consumeP(tokens);
        NodeExpr condition = parse_expr(tokens, token_length);

        if (peekP(tokens, token_length, 0).type != CLOSE_PAREN){
            printf("Missing close paren after if condition on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }

        consumeP(tokens);

        if (peekP(tokens, token_length, 0).type != OPEN_CURLY){
            printf("Missing open curly brace after if condition on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }

        consumeP(tokens);

        int thenCapacity = 10;
        NodeStmt* tempThen = (NodeStmt*)malloc(sizeof(NodeStmt) * thenCapacity);
        int thenCount = 0;

        while (peekP(tokens, token_length, 0).type != CLOSE_CURLY) {
            if (thenCount >= thenCapacity) {
                thenCapacity *= 2;
                tempThen = (NodeStmt*)realloc(tempThen, sizeof(NodeStmt) * thenCapacity);
                if (!tempThen) {
                    printf("Failed to reallocate memory for then statements\n");
                    exit(1);
                }
            }
            tempThen[thenCount++] = parse_stmt(tokens, token_length);
        }

        consumeP(tokens);

        NodeStmt* elseStmt = NULL;
        int elseCount = 0;

        if (peekP(tokens, token_length, 0).type == ELSE) {
            consumeP(tokens);
            if (peekP(tokens, token_length, 0).type != OPEN_CURLY) {
                printf("Missing open curly brace after else on line %d\n", peekP(tokens, token_length, 0).line);
                exit(1);
            }

            consumeP(tokens);

            int elseCapacity = 10;
            elseStmt = (NodeStmt*)malloc(sizeof(NodeStmt) * elseCapacity);
            elseCount = 0;

            while (peekP(tokens, token_length, 0).type != CLOSE_CURLY) {
                if (elseCount >= elseCapacity) {
                    elseCapacity *= 2;
                    elseStmt = (NodeStmt*)realloc(elseStmt, sizeof(NodeStmt) * elseCapacity);
                    if (!elseStmt) {
                        printf("Failed to reallocate memory for else statements\n");
                        exit(1);
                    }
                }
                elseStmt[elseCount++] = parse_stmt(tokens, token_length);
            }
            consumeP(tokens);           
        }

        node.type = NODE_STMT_IF;
        node.data.if_in.condition = condition;
        node.data.if_in.ifStmts = tempThen;
        node.data.if_in.thenCount = thenCount;
        node.data.if_in.elseStmts = elseStmt;
        node.data.if_in.elseCount = elseCount;
        
    }
    else if (peekP(tokens, token_length, 0).type == WHILE){
        consumeP(tokens);
        if (peekP(tokens, token_length, 0).type != OPEN_PAREN){
            printf("Missing open paren after while on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }
        consumeP(tokens);
        NodeExpr condition = parse_expr(tokens, token_length);

        if (peekP(tokens, token_length, 0).type != CLOSE_PAREN){
            printf("Missing close paren after while condition on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }

        consumeP(tokens);

        if (peekP(tokens, token_length, 0).type != OPEN_CURLY){
            printf("Missing open curly brace after while condition on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }

        consumeP(tokens);

        int stmtCapacity = 10;
        NodeStmt* tempStmt = (NodeStmt*)malloc(sizeof(NodeStmt) * stmtCapacity);
        int stmtCount = 0;

        while (peekP(tokens, token_length, 0).type != CLOSE_CURLY) {
            if (stmtCount >= stmtCapacity) {
                stmtCapacity *= 2;
                tempStmt = (NodeStmt*)realloc(tempStmt, sizeof(NodeStmt) * stmtCapacity);
                if (!tempStmt) {
                    printf("Failed to reallocate memory for while statements\n");
                    exit(1);
                }
            }
            tempStmt[stmtCount++] = parse_stmt(tokens, token_length);
        }

        consumeP(tokens);

        node.type = NODE_STMT_WHILE;
        node.data.while_in.condition = condition;
        node.data.while_in.stmts = tempStmt;
        node.data.while_in.stmt_count = stmtCount;
    }
    else if (peekP(tokens, token_length, 0).type == LET){
        consumeP(tokens);
        NodeExpr name = parse_expr(tokens, token_length);

        if (peekP(tokens, token_length, 0).type != COLON){
            printf("Missing colon after let statement on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }
        consumeP(tokens);

        Token typeToken = consumeP(tokens);
        if (typeToken.type != INT && typeToken.type != VOID){
            printf("Unsupported type in let statement on line %d\n", typeToken.line);
            exit(1); 
        }

        char* type = TokenToString(typeToken.type);

        NodeExpr initExpr;
        int hasInit = 0;

        if (peekP(tokens, token_length, 0).type == EQUAL) {
            consumeP(tokens);
            initExpr = parse_expr(tokens, token_length);
            hasInit = 1;
        }

        if (peekP(tokens, token_length, 0).type != SEMICOLON){
            printf("Missing semi colon after let statement on line %d\n", peekP(tokens, token_length, 0).line);
            exit(1); 
        }
        consumeP(tokens);

        node.type = NODE_STMT_DECLARE;
        node.data.declare_in.name = name;
        node.data.declare_in.type = type;
        node.data.declare_in.optionalInit = hasInit ? &initExpr : NULL;

    }
    else{
        printf("Unexpected token type in statement on line %d\n", peekP(tokens, token_length, 0).line);
        exit(1);
    }
    return node;
}

NodeFunc parse_func(Token* tokens, int token_length){
    NodeFunc func;
    consumeP(tokens);
    NodeExpr name = parse_expr(tokens, token_length);
    func.name = name;
    
    consumeP(tokens);


    int argsCapacity = 10;
    NodeFuncArguments* tempArgs = malloc(sizeof(NodeFuncArguments) * argsCapacity);
    int arg_count = 0;
    while (peekP(tokens, token_length, 0).type != CLOSE_PAREN) {
        if (arg_count >= argsCapacity) {
            argsCapacity *= 2; 
            tempArgs = realloc(tempArgs, sizeof(NodeFuncArguments) * argsCapacity);
            if (!tempArgs) {
                exit(1); 
            }
        }
        NodeFuncArguments arg;
        arg.name = parse_expr(tokens, token_length); 
        consumeP(tokens); 
        arg.type = TokenToString(consumeP(tokens).type); 
        tempArgs[arg_count++] = arg;
        if (peekP(tokens, token_length, 0).type == COMMA) {
            consumeP(tokens); 
        }
    }
    consumeP(tokens);

    func.args = tempArgs;
    func.arg_count = arg_count;

    NodeStmt* tempStmts = malloc(sizeof(NodeStmt) * 10); 
    int stmt_count = 0;
    int stmtsCapacity = 10;

    // Consume arrow
    consumeP(tokens);
    
    Token returnToken = consumeP(tokens);
    switch (returnToken.type)
    {
    case INT:
        func.returnType = NODE_FUNC_INT;
        break;
    case VOID:
        func.returnType = NODE_FUNC_VOID;
        break;
    default:
        printf("Unsupported return type\n");
        exit(1);
    }

    consumeP(tokens);
    
    while (peekP(tokens, token_length, 0).type != CLOSE_CURLY) { 
        if (stmt_count >= stmtsCapacity) {
            stmtsCapacity *= 2; 
            tempStmts = realloc(tempStmts, sizeof(NodeStmt) * stmtsCapacity);
            if (!tempStmts) {
                exit(1); 
            }
        }
        NodeStmt stmt = parse_stmt(tokens, token_length);
        tempStmts[stmt_count++] = stmt;
    }

    consumeP(tokens);

    func.stmts = tempStmts;
    func.stmt_count = stmt_count;
    return func;   
}

NodeProg parseProg(Token* tokens, int token_length) {
    int stmtsCapacity = 10;
    NodeStmt* statements = (NodeStmt*)malloc(stmtsCapacity * sizeof(NodeStmt));
    if (!statements) {
        printf("Failed to allocate memory for statements\n");
        exit(1);
    }
    int stmtsLength = 0;

    int funcsCapacity = 10;
    NodeFunc* functions = (NodeFunc*)malloc(funcsCapacity * sizeof(NodeFunc));
    if (!functions) {
        printf("Failed to allocate memory for functions\n");
        exit(1);
    }
    int funcsLength = 0;

    while (indexP < token_length) {
        if (peekP(tokens, token_length, 0).type == EXIT || 
            peekP(tokens, token_length, 0).type == PRINTLN || 
            (peekP(tokens, token_length, 0).type == IDENTIFIER && peekP(tokens, token_length, 1).type == OPEN_PAREN) || 
            peekP(tokens, token_length, 0).type == RETURN ||
            (peekP(tokens, token_length, 0).type == IDENTIFIER && peekP(tokens, token_length, 1).type == EQUAL)) {
            if (stmtsLength >= stmtsCapacity) {
                stmtsCapacity *= 2;
                statements = (NodeStmt*)realloc(statements, stmtsCapacity * sizeof(NodeStmt));
                if (!statements) {
                    printf("Failed to reallocate memory for statements\n");
                    exit(1);
                }
            }
            statements[stmtsLength++] = parse_stmt(tokens, token_length);
        } else if (peekP(tokens, token_length, 0).type == FN) { 
            if (funcsLength >= funcsCapacity) {
                funcsCapacity *= 2;
                functions = (NodeFunc*)realloc(functions, funcsCapacity * sizeof(NodeFunc));
                if (!functions) {
                    printf("Failed to reallocate memory for functions\n");
                    exit(1);
                }
            }
            functions[funcsLength++] = parse_func(tokens, token_length);
        } else {
            indexP++; 
        }
    }

    NodeProg prog = {functions, statements, stmtsLength, funcsLength};
    return prog;
}