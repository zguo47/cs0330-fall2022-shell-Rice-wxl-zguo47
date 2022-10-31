#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

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
    
    // put tokens into argv
    for (int j=0; tokens[j] != NULL; j++) {
        argv[j] = tokens[j];
    }
    argv[i] = NULL;

    
    // deal with redirections

    // if (argv[0] == ">"){
    //     fprintf(stderr, "No command.")
    // }

    if (strcmp(argv[i-1], ">") == 0) {
        fprintf(stderr, "error: no redirection file specified.\n");
    }

    if (strcmp(argv[i-1], "<") == 0)  {
        fprintf(stderr, "error: no redirection file specified.\n");
    }

    if (strcmp(argv[i-1], ">>") == 0)  {
        fprintf(stderr, "error: no redirection file specified.\n");
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
                fprintf(stderr, "error: no Command.\n");
                return;
            }
        }
    }

    if ((size_re > 2)) {
        fprintf(stderr, "error: can't have two redirects on one line.\n");
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
    ssize_t bytesRead;
    while(1){

        #ifdef PROMPT 
        if (printf("33sh> ") < 0) { 
            fprintf(stderr, "error when printing prompt.\n");
        }
        if (fflush(stdout) < 0) {
            fprintf(stderr, "error when using fflush.\n");
        } 
        #endif


    
        char buffer[1024];
        memset(buffer, 0, 1024);

        char *tokens[512];
        memset(tokens, 0, 512 * sizeof(char *));

        char *argv[512];
        memset(argv, 0, 512 * sizeof(char *));

        char *redirect[512];
        memset(redirect, 0, 512 * sizeof(char *));

        char buf[1024];
    
        // strncpy(buffer, "> end", 1024);

        if ((bytesRead = read(0, buffer, 1024)) == -1) {
            perror("user input");
            continue; 
        }else if (bytesRead  == 0){
            exit(1);/* terminate shell */
        }else{
            parse(buffer, tokens, argv, redirect);
            // Handle the case with no input
            if (argv[0] == NULL) {
                continue;
            }
        }

        // command exit
        if (strcmp(argv[0], "exit") == 0) {
            exit(0);
        }

        // Child Process
        if (fork() == 0) {
        
            // command cd
            if (strcmp(argv[0], "cd") == 0){
                if (argv[1] == NULL) {
                    fprintf(stderr, "cd: syntax error \n");
                }
                else {
                    int cd = chdir(strcat(strcat(getcwd(buf, sizeof(buf)), "/"), argv[1]));
                    if (cd == -1) {
                        perror("cd");
                        exit(1);
                    }else{
                        continue;
                    }
                }
            }

            // command ln
            else if (strcmp(argv[0], "ln") == 0) {
                if (argv[1] == NULL || argv[2] == NULL) {
                fprintf(stderr, "ln: syntax error \n");
                }
                else {
                    if (link(argv[1],argv[2]) == -1) {
                        perror("ln");
                        exit(1);
               
                    }
                }
            }

            // command rm
            else if (strcmp(argv[0], "rm") == 0) {
                if (argv[1] == NULL) {
                    fprintf(stderr, "rm: syntax error \n");
                }
                else {
                    if (unlink(argv[1]) == -1) {
                        perror("rm");  
                        exit(1);
             
                    }
            
                }
            }    
            else {
                char *full_name = argv[0];
                if (strrchr(argv[0],'/') != NULL) {
                    argv[0] = strrchr(argv[0],'/') + 1;
                }
                execv(full_name, argv);
                perror("execv");
                exit(1);
            }
            exit(0);
        }


    // Parent Process continues
        wait(0);
    }
    return 0;
}
