#include <stdio.h>
#include <string.h>
#include <stddef.h>

void parse(char buffer[1024], char *tokens[512], char *argv[512], char *redirect[512]) {
    char *str = buffer;
    char *token;
    unsigned int i = 0;
    unsigned int size_re = 0;


    // tokenize
    while ((token = strtok(str, " \t\n")) != NULL) {
        tokens[i] = token;
        str = NULL;
        i +=1;
    }

    if (tokens[0] == NULL) {
        return;
    }
    
    // put tokens into argv while dealing with the file path
    for (int j=0; tokens[j] != NULL; j++) {
        if (strrchr(tokens[j],'/') != NULL) {
            argv[j] = strrchr(tokens[j],'/') + 1;
        } else {
            argv[j] = tokens[j];
        }
    }

    
    // deal with redirections

    // if (argv[0] == ">"){
    //     fprintf(stderr, "No command.")
    // }

    if (strcmp(argv[i-1], ">") == 0) {
        fprintf(stderr, "error: no redirection file specified.");
    }

    if (strcmp(argv[i-1], "<") == 0)  {
        fprintf(stderr, "error: no redirection file specified.");
    }

    if (strcmp(argv[i-1], ">>") == 0)  {
        fprintf(stderr, "error: no redirection file specified.");
    }

    for (unsigned int k=0; k < i-1; k++) {
        if (strcmp(argv[k], ">") == 0 || strcmp(argv[k], "<") == 0 || strcmp(argv[k], ">>") == 0) {
            redirect[size_re] = argv[k];
            redirect[size_re + 1] = argv[k+1];
            size_re += 2;
            memmove(argv + k, argv + k + 2, 8*(i-k-2));
            argv[i-2] = NULL;
            argv[i-1] = NULL;
            i -= 2;
            k -= 1;
            if (i == 0){
                fprintf(stderr, "error: no Command.");
                return;
            }
        }
    }

    if ((size_re > 2)) {
        fprintf(stderr, "error: can't have two redirects on one line.");
    }
    
}   

int main() {
    /* TODO: everything! */


    // while(1){
    //     input = readUserInput(stdin);
    //     if (input != EOF){
    //         parse(input);
    //     }else{
    //         exit(0);
    //     }
    // }

    // char str[1024] = "/wiojaoif/aifjaw/get cd 123 to abc < txt.txt end";
    
    char buffer[1024];
    memset(buffer, 0, 1024);

    char *tokens[512];
    memset(tokens, 0, 512 * sizeof(char *));

    char *argv[512];
    memset(argv, 0, 512 * sizeof(char *));

    char *redirect[512];
    memset(redirect, 0, 512 * sizeof(char *));
    
    strncpy(buffer, "> end", 1024);

    parse(buffer, tokens, argv, redirect);



    if (argv[0] == "cd"){
        if (argv[1] == NULL) {
            fprintf(stderr, "cd: syntax error");
        }
        else {
            if (chdir(argv[1]) == -1) {
                fprintf(stderr, "cd: no such file or directory")''
            }
        }
    }

    else if (argv[0] == "ln") {
        if (argv[1] == NULL) {
            fprintf(stderr, "ln: syntax error");
        }
        else {
            
        }
    }
    else if (argv[0] == "rm") {
        if (argv[1] == NULL) {
            fprintf(stderr, "ln: syntax error");
        }
        else {
            
        }
    }    
    else if (argv[0] == "exit") {

    }
    else {

    }


    return 0;
}