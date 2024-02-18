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
                exit(1); // Or handle the error gracefully
            }
        }
        case NODE_EXPR_NUMB:
            result = expr;
            break;
        default:
            printf("Unsupported expression type for evaluation.\n");
            exit(1);
    }
    return result;
}

void InterpetStatements(NodeStmt stmt, Scope* scope){
    switch (stmt.type)
    {
    case NODE_STMT_EXIT:
        exit(stmt.data.exit_in.code.data.numb.value);
        break;
    case NODE_STMT_PRINTLN:
        if (stmt.data.printf_in.data.type == NODE_EXPR_NUMB){
            printf("%d\n", stmt.data.printf_in.data.data.numb.value);
        }
        else if (stmt.data.printf_in.data.type == NODE_EXPR_IDENT){
            NodeExpr* value = getVariable(scope, stmt.data.printf_in.data.data.ident.value);
            if (value) {
                printf("%d\n", value->data.numb.value);
            } else {
                printf("Identifier '%s' not found.\n", stmt.data.printf_in.data.data.ident.value);
                exit(1);
            }
        }
        break;
    case NODE_STMT_FUNCALL: {
            NodeFunc* func = findFunction(stmt.data.func_call.name.data.ident.value);
            if (func == NULL) {
                printf("Function not found.\n");
                break;
            }

            Scope* functionScope = createScope(scope);

            for (int i = 0; i < stmt.data.func_call.argCount && i < func->arg_count; i++) {
                NodeExpr evaluatedArg = EvaluateExpression(stmt.data.func_call.args[i], scope);
                setVariable(functionScope, func->args[i].name.data.ident.value, evaluatedArg);
            }

            for (int i = 0; i < func->stmt_count; i++) {
                InterpetStatements(func->stmts[i], functionScope);
            }

            destroyScope(functionScope);
            break;
        }
    }
}

void Interpet(NodeProg prog) {
    Scope* globalScope = createScope(NULL);

    for (int i = 0; i < prog.fun_length; i++) {
        addFunction(prog.functions[i]);
    }

    for (int i = 0; i < prog.length; i++) {
        InterpetStatements(prog.smts[i], globalScope); 
    }

    destroyScope(globalScope);
}
