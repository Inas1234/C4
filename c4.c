#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "./include/Tokenizer.h"
#include "./include/Parser.h"
#include "./include/Interpreter.h"
#include "./include/Compile.h"

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


char *remove_ext (char* myStr, char extSep, char pathSep) {
    char *retStr, *lastExt, *lastPath;


    if (myStr == NULL) return NULL;
    if ((retStr = malloc (strlen (myStr) + 1)) == NULL) return NULL;


    strcpy (retStr, myStr);
    lastExt = strrchr (retStr, extSep);
    lastPath = (pathSep == 0) ? NULL : strrchr (retStr, pathSep);


    if (lastExt != NULL) {

        if (lastPath != NULL) {
            if (lastPath < lastExt) {

                *lastExt = '\0';
            }
        } else {

            *lastExt = '\0';
        }
    }


    return retStr;
}


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: main <file> [-d] [-i] [-c]\n");
        exit(1);
    }

    bool debug = false;
    bool interpret = false;
    bool compile = false;

    char *file_path = NULL;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            debug = true;
        } else if (strcmp(argv[i], "-i") == 0) {
            interpret = true;
        } else if (strcmp(argv[i], "-c") == 0) {
            compile = true;
        } else {
            file_path = argv[i];
        }
    }

    if (file_path == NULL) {
        printf("Error: No file specified.\n");
        printf("Usage: main <file> [-d] [-i] [-c]\n");
        exit(1);
    }

    char *contents = readFile(file_path);
    if (contents == NULL) {
        exit(1);
    }

    int token_len = 0;
    Token* tokens = tokenize(contents, &token_len);

    NodeProg prog = parseProg(tokens, token_len);

    if (debug) {
        printf("Debug mode enabled\n");
        printf("TOKENS_SIZE: %d\n", token_len);
        printNodeProg(prog);
    }

    if (interpret) {
        printf("Interpreting...\n");
        Interpet(prog);
    }

    if (compile) {
        printf("Compiling...\n");
        FILE *outputFile = fopen("output.c", "w");
        if (outputFile == NULL) {
            perror("Failed to open output file");
            exit(1);
        }
        compileProg(prog, outputFile);
        fclose(outputFile);
        char command[100];
        strcpy(command, "gcc output.c -o ");
        char *output_name = remove_ext(file_path, '.', '/');
        strcat(command, output_name);
        system(command);
        printf("Compilation successful. Output written to %s\n", output_name);
        remove("output.c");
        free(output_name);
    }

    free(contents);
    free(tokens);
    free(prog.smts);
    return 0;
}
