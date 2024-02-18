#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parser.h"

typedef struct Variable {
    char* name;
    NodeExpr value;
    struct Variable* next;
} Variable;

typedef struct Scope {
    struct Scope* parent;
    Variable* variables;
} Scope;


Scope* createScope(Scope* parent) {
    Scope* scope = (Scope*)malloc(sizeof(Scope));
    if (!scope) {
        perror("Failed to create scope");
        exit(EXIT_FAILURE);
    }
    scope->parent = parent;
    scope->variables = NULL;
    return scope;
}

void destroyScope(Scope* scope) {
    Variable* current = scope->variables;
    while (current != NULL) {
        Variable* toDelete = current;
        current = current->next;
        free(toDelete->name);
        free(toDelete);
    }
    free(scope);
}

void setVariable(Scope* scope, char* name, NodeExpr value) {
    // Check if variable already exists
    for (Variable* var = scope->variables; var != NULL; var = var->next) {
        if (strcmp(var->name, name) == 0) {
            var->value = value; // Update existing variable
            return;
        }
    }

    // Create new variable
    Variable* newVar = (Variable*)malloc(sizeof(Variable));
    if (!newVar) {
        perror("Failed to set variable");
        exit(EXIT_FAILURE);
    }
    newVar->name = strdup(name);
    newVar->value = value;
    newVar->next = scope->variables;
    scope->variables = newVar;
}

NodeExpr* getVariable(Scope* scope, char* name) {
    Scope* currentScope = scope;
    while (currentScope != NULL) {
        for (Variable* var = currentScope->variables; var != NULL; var = var->next) {
            if (strcmp(var->name, name) == 0) {
                return &var->value;
            }
        }
        currentScope = currentScope->parent;
    }
    return NULL; // Variable not found
}
