#pragma once
#include "Parser.h"


void InterpetStatements(NodeStmt stmt){
    switch (stmt.type)
    {
    case NODE_STMT_EXIT:
        exit(stmt.data.exit_in.code.data.numb.value);
        break;
    case NODE_STMT_PRINTLN:
        if (stmt.data.printf_in.data.type == NODE_EXPR_NUMB){
            printf("%d", stmt.data.printf_in.data.data.numb.value);
        }
        break;
    default:
        break;
    }
}

void Interpet(NodeProg prog){
    for (int i =0; i < prog.length; i++){
        InterpetStatements(prog.smts[i]);
    }

}