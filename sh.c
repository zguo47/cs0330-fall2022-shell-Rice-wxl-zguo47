#include <stdio.h>

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

    if (argv[sizeof(argv)-1] == ">") {
        fprintf(stderr, "No redirection file specified.")
    }

    if (argv[sizeof(argv)-1] == "<")  {
        fprintf(stderr, "No redirection file specified.")
    }

    if (argv[sizeof(argv)-1] == ">>")  {
        fprintf(stderr, "No redirection file specified.")
    }

    for (int i=0; i<sizeof(argv)-1; i++) {
        if (argv[i] == ">" || argv[i] == "<" || argv[i] == ">>") {
            strncat(redirect, argv + i, 2)
            memmove(argv + i, argv + i + 2, strlen(argv) - i)
        }
    }

    if (sizeof(redirect) > 2) {
        fprintf(stderr, "Can't have two redirects on one line.")
    }
}

int main() {
    /* TODO: everything! */
    while(1){
        input = readUserInput(stdin);
        if (input != EOF){
            parse(input);
        }else{
            exit(0);
        }
    }

    return 0;
}
