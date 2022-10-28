#include <stdio.h>
#include <string.h>
#include <stddef.h>

void parse(char buffer[1024], char *tokens[512], char *argv[512], char *redirect[512]) {
    char *str = buffer;
    char *token;
    int i = 0;


    while ((token = strtok(str, " \t\n")) != NULL) {
        tokens[i] = token;
        str = NULL;
        i +=1;
    }

    if (tokens[0] == NULL) {
        return;
    }
    
    // deal with the file path
    if (strrchr(tokens[0],'/') != NULL) {
        argv[0] = strrchr(tokens[0],'/') + 1;
    } else {
        argv[0] = tokens[0];
    }

    // put tokens into argv
    for (int j=1; tokens[j] != NULL; j++) {
        argv[j] = tokens[j];
    }
    
    // deal with redirections

    // if (argv[0] == ">"){
    //     fprintf(stderr, "No command.")
    // }

    if (strcmp(argv[sizeof(*argv)-1], ">") == 0) {
        fprintf(stderr, "No redirection file specified.");
    }

    if (strcmp(argv[sizeof(*argv)-1], "<") == 0)  {
        fprintf(stderr, "No redirection file specified.");
    }

    if (strcmp(argv[sizeof(*argv)-1], ">>") == 0)  {
        fprintf(stderr, "No redirection file specified.");
    }

    for (unsigned int i=0; i < sizeof(*argv)-1; i++) {
        if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], "<") == 0 || strcmp(argv[i], ">>") == 0) {
            // strncat(*redirect, argv[i], 2);
            memmove(argv[i], argv[i+2], strlen(*argv)-2);
        }
    }

    if (sizeof(*redirect) > 2) {
        fprintf(stderr, "Can't have two redirects on one line.");
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
    
    strncpy(buffer, "/wiojaoif/aifjaw/get cd 123 to abc < txt.txt end", 1024);

    parse(buffer, tokens, argv, redirect);
    printf(*argv, redirect);
    return 0;
}
