#include <stdio.h>
#include <stdlib.h>
#include "./include/Tokenizer.h"
#include "./include/Parser.h"
#include "./include/Interpreter.h"

char *readFile(char *file_path) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        perror("Failed to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long s = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(s + 1); 
    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }


    size_t bytesRead = fread(buffer, 1, s, file);
    if (bytesRead < s) {
        perror("Failed to read the entire file");
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[s] = '\0'; 
    fclose(file);
    return buffer;
}


int main(int argc, char**argv){
    if (argc < 2){
        printf("Usage: main <file>\n");
        exit(1);
    }

    char * contents = readFile(argv[1]);

    int token_len = 0;
    Token* tokens = tokenize(contents, &token_len);  
    printf("TOKENS_SIZE: %d\n", token_len);


    NodeProg prog = parseProg(tokens, token_len);
    printNodeProg(prog);
    Interpet(prog);

    free(contents);
    free (tokens);
    free(prog.smts);
    return 0;
}