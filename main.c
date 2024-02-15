#include <stdio.h>
#include <stdlib.h>
#include "./include/Tokenizer.h"
#include "./include/Parser.h"

char *readFile() {
    FILE *file = fopen("test.my", "rb");
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


int main(void){
    char * contents = readFile();
    int token_len = 0;
    Token* tokens = tokenize(contents, &token_len);  
    printf("TOKENS_SIZE: %d\n", token_len);


    NodeProg prog = parseProg(tokens, token_len);
    printNodeProg(prog);
    

    free(contents);
    free (tokens);
    free(prog.smts);
    return 0;
}