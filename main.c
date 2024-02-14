#include <stdio.h>
#include <stdlib.h>
#include "./include/Tokenizer.h"

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
    printf("%s\n", contents);
    int token_len = 0;
    Token* tokens = tokenize(contents, &token_len);  

    for (int i =0; i < token_len; i++){
        printf("Token: %s Value: %s \n", TokenToString(tokens[i].type), tokens[i].value);
    }

    free(contents);
    free (tokens);
    return 0;
}