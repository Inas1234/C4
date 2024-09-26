#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    NUMBER,
    LET,
    INT,
    VOID,
    IDENTIFIER,
    EQUAL,
    END,
    COLON,
    SEMICOLON,
    EXIT,
    OPEN_PAREN,
    CLOSE_PAREN,
    PRINTLN,
    FN,
    COMMA,
    OPEN_CURLY,
    CLOSE_CURLY,
    ARROW,
    RETURN,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_EQUAL,
    GREATER_THAN_EQUAL,
    EQUAL_EQUAL,
    NOT_EQUAL,
    IF,
    ELSE,
    WHILE,    
}TokenType;


typedef struct {
    TokenType type;
    char * value;
    int line;
} Token;

static int indexT = 0;

char peek(char * contents, int ahead) {
    if (indexT + ahead >= strlen(contents)){
        return '\0';
    }
    return contents[indexT + ahead];    
}
char consume(char *contents){
    return contents[indexT++];
};


char *TokenToString(TokenType type){
    switch (type)
    {
    case NUMBER: return "NUMBER";
    case LET: return "LET";
    case INT: return "int";
    case IDENTIFIER: return "IDENTIFIER";
    case END: return "END";
    case COLON: return "COLON";
    case EQUAL: return "EQUAL";
    case SEMICOLON: return "SEMICOLON";
    case EXIT: return "EXIT";
    case OPEN_PAREN: return "OPEN_PAREN";
    case CLOSE_PAREN: return "CLOSE_PAREN";
    case PRINTLN: return "PRINTLN";
    case VOID: return "void";
    case RETURN: return "RETURN";    
    default:
        break;
    }
}



Token* tokenize(char* contents, int *tokenCount){
    Token* tokens = (Token*)malloc(sizeof(Token) * strlen(contents));
    char * buffer = (char*)malloc(256);

    int buffer_length = 0, line=1;
    *tokenCount = 0;
    while (peek(contents, 0) != '\0')
    {
        if (isalpha(peek(contents, 0))){
            buffer_length = 0;
            buffer[buffer_length++] = consume(contents);
            while (isalnum(peek(contents, 0)))
            {
                buffer[buffer_length++] = consume(contents);
            }
            buffer[buffer_length] = '\0';

            if (strncmp(buffer, "let", 3) == 0){
                tokens[(*tokenCount)++] = (Token){LET, NULL, line};
                buffer[0] = '\0';
            }  
            else if (strncmp(buffer, "int", 3) == 0){
                tokens[(*tokenCount)++] = (Token){INT, NULL, line};
                buffer[0] = '\0';
            }  
            else if (strncmp(buffer, "exit", 3) == 0){
                tokens[(*tokenCount)++] = (Token){EXIT, NULL, line};
                buffer[0] = '\0';
            }
            else if (strncmp(buffer, "println", 6) == 0){
                tokens[(*tokenCount)++] = (Token){PRINTLN, NULL, line};
                buffer[0] = '\0';
            }
            else if (strncmp(buffer, "fn", 2) == 0){
                tokens[(*tokenCount)++] = (Token){FN, NULL, line};
                buffer[0] = '\0';
            }
            else if (strncmp(buffer, "void", 4) == 0){
                tokens[(*tokenCount)++] = (Token){VOID, NULL, line};
                buffer[0] = '\0';
            }
            else if (strncmp(buffer, "return", 6) == 0){
                tokens[(*tokenCount)++] = (Token){RETURN, NULL, line};
                buffer[0] = '\0';
            }
            else if (strcmp(buffer, "if") == 0){
                tokens[(*tokenCount)++] = (Token){IF, NULL, line};
                buffer[0] = '\0';
            }
            else if (strcmp(buffer, "else") == 0){
                tokens[(*tokenCount)++] = (Token){ELSE, NULL, line};
                buffer[0] = '\0';
            }
            else if (strcmp(buffer, "while") == 0){
                tokens[(*tokenCount)++] = (Token){WHILE, NULL, line};
                buffer[0] = '\0';
            }
            else {
                tokens[(*tokenCount)++] = (Token){IDENTIFIER, strdup(buffer), line};
                buffer[0] = '\0';
            }
        }
        else if (isdigit(peek(contents, 0))){
            buffer_length = 0;
            buffer[buffer_length++] = consume(contents);
            while (isdigit(peek(contents, 0)))
            {
                buffer[buffer_length++] = consume(contents);
            }
            buffer[buffer_length] = '\0';

            tokens[(*tokenCount)++] = (Token){NUMBER, strdup(buffer), line};
            buffer[0] = '\0';
        }
        else if (peek(contents, 0) == '='){
            consume(contents);
            if (peek(contents, 0) == '='){
                consume(contents);
                tokens[(*tokenCount)++] = (Token){EQUAL_EQUAL, NULL, line};
            }
            else{
                tokens[(*tokenCount)++] = (Token){EQUAL, NULL, line};
            }
        }
        else if (peek(contents, 0) == ':'){
            consume(contents);
            tokens[(*tokenCount)++] = (Token){COLON, NULL, line};
        }
        else if (peek(contents, 0) == ';') {
            consume(contents);
            tokens[(*tokenCount)++] = (Token){SEMICOLON, NULL, line};
        }
        else if (peek(contents, 0) == '('){
            consume(contents);
            tokens[(*tokenCount)++] = (Token){OPEN_PAREN, NULL, line};

        }
        else if (peek(contents, 0) == ')'){
            consume(contents);
            tokens[(*tokenCount)++] = (Token){CLOSE_PAREN, NULL, line};
        }
        else if (peek(contents, 0) == '{'){
            consume(contents);
            tokens[(*tokenCount)++] = (Token){OPEN_CURLY, NULL, line};

        }
        else if (peek(contents, 0) == '}'){
            consume(contents);
            tokens[(*tokenCount)++] = (Token){CLOSE_CURLY, NULL, line};
        }
        else if (peek(contents, 0) == ','){
            consume(contents);
            tokens[(*tokenCount)++] = (Token){COMMA, NULL, line};
        }
        else if (peek(contents, 0) == '-'){
            consume(contents);
            if (peek(contents, 0) == '>'){
                consume(contents);
                tokens[(*tokenCount)++] = (Token){ARROW, NULL, line};
            }
        }
        else if (peek(contents, 0) == '+'){
            consume(contents);
            tokens[(*tokenCount)++] = (Token){PLUS, NULL, line};
        }
        else if (peek(contents, 0) == '-'){
            consume(contents);
            tokens[(*tokenCount)++] = (Token){MINUS, NULL, line};
        }
        else if (peek(contents, 0) == '*'){
            consume(contents);
            tokens[(*tokenCount)++] = (Token){MULTIPLY, NULL, line};
        }
        else if (peek(contents, 0) == '/'){
            consume(contents);
            tokens[(*tokenCount)++] = (Token){DIVIDE, NULL, line};
        }
        else if (peek(contents, 0) == '<' ){
            consume(contents);
            if (peek(contents, 0) == '='){
                consume(contents);
                tokens[(*tokenCount)++] = (Token){LESS_THAN_EQUAL, NULL, line};
            }
            else {
                tokens[(*tokenCount)++] = (Token){LESS_THAN, NULL, line};
            }
        }
        else if (peek(contents, 0) == '>' ){
            consume(contents);
            if (peek(contents, 0) == '='){
                consume(contents);
                tokens[(*tokenCount)++] = (Token){GREATER_THAN_EQUAL, NULL, line};
            }
            else {
                tokens[(*tokenCount)++] = (Token){GREATER_THAN, NULL, line};
            }
        }

        else if (peek(contents, 0) == '!'){
            consume(contents);
            if (peek(contents, 0) == '='){
                consume(contents);
                tokens[(*tokenCount)++] = (Token){NOT_EQUAL, NULL, line};
            }
        }
        else if (isspace(peek(contents, 0))){
            consume(contents);
        }
        else if (peek(contents, 0) == '\n') {
            line++;
            consume(contents); 
            continue; 
        }
        else {
            printf("UNKNOWN CHARACTER DETECTED: %c\n", peek(contents, 0));
            exit(EXIT_FAILURE);
        }

    }
    indexT = 0;
        
    tokens[(*tokenCount)] = (Token){END, NULL, line};
    free(buffer);
    return tokens;
    
}

