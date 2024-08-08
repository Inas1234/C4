#pragma once
#include "Parser.h"

void compileExpr(NodeExpr expr, FILE* outputFile) {
    switch (expr.type) {
        case NODE_EXPR_IDENT:
            fprintf(outputFile, "%s", expr.data.ident.value);
            break;
        case NODE_EXPR_NUMB:
            fprintf(outputFile, "%d", expr.data.numb.value);
            break;
        default:
            fprintf(stderr, "Unknown expression type\n");
            exit(1);
    }
}

void compileFuncCall(NodeStmtFuncCall func_call, FILE* outputFile) {
    compileExpr(func_call.name, outputFile);
    fprintf(outputFile, "(");
    for (int i = 0; i < func_call.argCount; i++) {
        if (i > 0) {
            fprintf(outputFile, ", ");
        }
        compileExpr(func_call.args[i], outputFile);
    }
    fprintf(outputFile, ")");
}

void compileStmt(NodeStmt stmt, FILE* outputFile) {
    switch (stmt.type) {
        case NODE_STMT_EXIT:
            fprintf(outputFile, "exit(");
            compileExpr(stmt.data.exit_in.code, outputFile);
            fprintf(outputFile, ");\n");
            break;
        case NODE_STMT_PRINTLN:
            fprintf(outputFile, "printf(\"%%d\\n\", ");
            if (stmt.data.printf_in.func_call != NULL) {
                compileFuncCall(*stmt.data.printf_in.func_call, outputFile);
            } else {
                compileExpr(stmt.data.printf_in.data, outputFile);
            }
            fprintf(outputFile, ");\n");
            break;
        case NODE_STMT_FUNCALL:
            compileFuncCall(stmt.data.func_call, outputFile);
            fprintf(outputFile, ";\n");
            break;
        case NODE_STMT_RETURN:
            fprintf(outputFile, "return ");
            compileExpr(stmt.data.return_in.value, outputFile);
            fprintf(outputFile, ";\n");
            break;
        case NODE_STMT_ASSIGN:
            compileExpr(stmt.data.assign_in.left, outputFile);
            fprintf(outputFile, " = ");
            compileExpr(stmt.data.assign_in.right, outputFile);
            fprintf(outputFile, ";\n");
            break;
        default:
            fprintf(stderr, "Unknown statement type\n");
            exit(1);
    }
}


const char* toLowerCase(const char* str) {
    char* result = (char*)malloc(strlen(str) + 1);
    for (int i = 0; i < strlen(str); i++) {
        result[i] = tolower(str[i]);
    }
    result[strlen(str)] = '\0';
    return result;
}


void compileFunc(NodeFunc func, FILE* outputFile) {
    fprintf(outputFile, "%s ", func.returnType == NODE_FUNC_INT ? "int" : "void");

    compileExpr(func.name, outputFile);

    fprintf(outputFile, "(");
    for (int i = 0; i < func.arg_count; i++) {
        if (i > 0) {
            fprintf(outputFile, ", ");
        }
        fprintf(outputFile, "%s %s", toLowerCase(func.args[i].type), func.args[i].name.data.ident.value);
    }
    fprintf(outputFile, ") {\n");

    for (int i = 0; i < func.stmt_count; i++) {
        compileStmt(func.stmts[i], outputFile);
    }
    fprintf(outputFile, "}\n");
}


void compileProg(NodeProg prog, FILE* outputFile) {
    fprintf(outputFile, "#include <stdio.h>\n");
    fprintf(outputFile, "#include <stdlib.h>\n");
    fprintf(outputFile, "#include <stdbool.h>\n");
    fprintf(outputFile, "#include <string.h>\n");

    for (int i = 0; i < prog.fun_length; i++) {
        compileFunc(prog.functions[i], outputFile);
    }

    fprintf(outputFile, "int main() {\n");
    for (int i = 0; i < prog.length; i++) {
        compileStmt(prog.smts[i], outputFile);
    }
    fprintf(outputFile, "return 0;\n");
    fprintf(outputFile, "}\n");
}
