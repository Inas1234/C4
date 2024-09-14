#pragma once
#include "Parser.h"
#include "Scopes.h"

#define MAX_FUNCTIONS 100
NodeFunc functionTable[MAX_FUNCTIONS];
int functionCount = 0;

void addFunction(NodeFunc func) {
    if (functionCount < MAX_FUNCTIONS) {
        functionTable[functionCount++] = func;
    } else {
        printf("Function table is full.\n");
    }
}

NodeFunc* findFunction(char* name) {
    for (int i = 0; i < functionCount; i++) {
        if (strcmp(functionTable[i].name.data.ident.value, name) == 0) {
            return &functionTable[i];
        }
    }
    return NULL;
}

NodeExpr EvaluateExpression(NodeExpr expr, Scope* scope) {
    NodeExpr result;

    switch (expr.type) {
        case NODE_EXPR_IDENT: {
            NodeExpr* value = getVariable(scope, expr.data.ident.value);
            if (value) {
                return *value;
            } else {
                printf("Identifier '%s' not found.\n", expr.data.ident.value);
                exit(1); 
            }
        }
        case NODE_EXPR_NUMB:
            result = expr;
            break;
        case NODE_EXPR_BINARY: {
            BinaryExpressionPlus* binaryExpr = (BinaryExpressionPlus*)expr.data.ident.value;
            NodeExpr left = EvaluateExpression(*binaryExpr->left, scope);
            NodeExpr right = EvaluateExpression(*binaryExpr->right, scope);

            if (left.type != NODE_EXPR_NUMB || right.type != NODE_EXPR_NUMB) {
                printf("Binary expressions currently only support numeric values.\n");
                exit(1);
            }

            result.type = NODE_EXPR_NUMB;
            switch (binaryExpr->token.type) {
                case PLUS:
                    result.data.numb.value = left.data.numb.value + right.data.numb.value;
                    break;
                case MINUS:
                    result.data.numb.value = left.data.numb.value - right.data.numb.value;
                    break;
                case MULTIPLY:
                    result.data.numb.value = left.data.numb.value * right.data.numb.value;
                    break;
                case DIVIDE:
                    if (right.data.numb.value == 0) {
                        printf("Division by zero error.\n");
                        exit(1);
                    }
                    result.data.numb.value = left.data.numb.value / right.data.numb.value;
                    break;
                case LESS_THAN:
                    result.data.numb.value = left.data.numb.value < right.data.numb.value;
                    break;
                case GREATER_THAN:
                    result.data.numb.value = left.data.numb.value > right.data.numb.value;
                    break;
                case LESS_THAN_EQUAL:
                    result.data.numb.value = left.data.numb.value <= right.data.numb.value;
                    break;
                case GREATER_THAN_EQUAL:
                    result.data.numb.value = left.data.numb.value >= right.data.numb.value;
                    break;
                case EQUAL_EQUAL:
                    result.data.numb.value = left.data.numb.value == right.data.numb.value;
                    break;
                case NOT_EQUAL:
                    result.data.numb.value = left.data.numb.value != right.data.numb.value;
                    break;
                default:
                    printf("Unsupported binary operator.\n");
                    exit(1);
            }
            break;
        }
        default:
            printf("Unsupported expression type for evaluation.\n");
            exit(1);
    }

    return result;
}

void InterpetStatements(NodeStmt stmt, Scope* scope, NodeFunc* current_func){

    switch (stmt.type)
    {
    case NODE_STMT_EXIT:
        exit(stmt.data.exit_in.code.data.numb.value);
        break;
    case NODE_STMT_PRINTLN:
        if (stmt.data.printf_in.func_call != NULL) {
            NodeFunc* func = findFunction(stmt.data.printf_in.func_call->name.data.ident.value);
            if (func == NULL) {
                printf("Function %s not found.\n", stmt.data.printf_in.func_call->name.data.ident.value);
                break;
            }

            Scope* functionScope = createScope(scope);

            for (int i = 0; i < stmt.data.printf_in.func_call->argCount && i < func->arg_count; i++) {
                NodeExpr evaluatedArg = EvaluateExpression(stmt.data.printf_in.func_call->args[i], scope);
                setVariable(functionScope, func->args[i].name.data.ident.value, evaluatedArg);
            }

            NodeExpr returnVal;
            returnVal.type = NODE_EXPR_NUMB;

            for (int i = 0; i < func->stmt_count; i++) {
                InterpetStatements(func->stmts[i], functionScope, func);

                if (func->stmts[i].type == NODE_STMT_RETURN) {
                    returnVal = EvaluateExpression(func->stmts[i].data.return_in.value, functionScope);
                    break;
                }
            }

            if (func->returnType == NODE_FUNC_INT) {
                printf("%d\n", returnVal.data.numb.value);
            } else if (func->returnType == NODE_FUNC_VOID) {
                printf("Cannot print void values yet.\n");
            }

            destroyScope(functionScope);
        } else if (stmt.data.printf_in.data.type == NODE_EXPR_IDENT) {
            // Printing an identifier's value
            NodeExpr* value = getVariable(scope, stmt.data.printf_in.data.data.ident.value);
            if (value) {
                printf("%d\n", value->data.numb.value);
            } else {
                printf("Identifier '%s' not found.\n", stmt.data.printf_in.data.data.ident.value);
                exit(1);
            }
        } else if (stmt.data.printf_in.data.type == NODE_EXPR_NUMB) {
            // Printing a number
            printf("Printing number: %d\n", stmt.data.printf_in.data.data.numb.value);
        } else {
            printf("Unhandled case in PRINTLN.\n");
        }
        break;
    case NODE_STMT_FUNCALL: {
            NodeFunc* func = findFunction(stmt.data.func_call.name.data.ident.value);
            if (func == NULL) {
                printf("Function %s not found.\n", stmt.data.func_call.name.data.ident.value);
                break;
            }

            Scope* functionScope = createScope(scope);

            for (int i = 0; i < stmt.data.func_call.argCount && i < func->arg_count; i++) {
                NodeExpr evaluatedArg = EvaluateExpression(stmt.data.func_call.args[i], scope);
                setVariable(functionScope, func->args[i].name.data.ident.value, evaluatedArg);
            }

            NodeExpr returnVal;
            returnVal.type = NODE_EXPR_NUMB;

            for (int i = 0; i < func->stmt_count; i++) {
                InterpetStatements(func->stmts[i], functionScope, func);

                if (func->stmts[i].type == NODE_STMT_RETURN) {
                    returnVal = EvaluateExpression(func->stmts[i].data.return_in.value, functionScope);
                    break;
                }
            }

            if (current_func != NULL) {
                current_func->returnValue = returnVal;
                printf("Set return value for current function: %d\n", returnVal.data.numb.value);
            }

            destroyScope(functionScope);
            break;
        }
    case NODE_STMT_RETURN:
        if (current_func != NULL) {
            current_func->returnValue = EvaluateExpression(stmt.data.return_in.value, scope);
        }
        return;
    case NODE_STMT_ASSIGN:
        setVariable(scope, stmt.data.assign_in.left.data.ident.value, EvaluateExpression(stmt.data.assign_in.right, scope));
        break;  
    }
}

void Interpet(NodeProg prog) {
    Scope* globalScope = createScope(NULL);

    for (int i = 0; i < prog.fun_length; i++) {
        addFunction(prog.functions[i]);
    }

    for (int i = 0; i < prog.length; i++) {
        InterpetStatements(prog.smts[i], globalScope, NULL); 
    }

    destroyScope(globalScope);
}
