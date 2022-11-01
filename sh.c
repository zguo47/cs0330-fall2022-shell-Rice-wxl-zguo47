#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * parse()
 *
 * - Description: creates the tokens, argv, and redirect arrays from the buffer
 * character array
 *
 * - Arguments: buffer: a char array representing user input, tokens: the
 * tokenized input, argv: the argument array eventually used for execv(),
 * redirect: the array storing the redirection information
 *
 * - Return: a number indicating whether there is an error when parsing the
 * input. 0 is returned if the parse succeeds, and 1 is returned if there is an
 * error.
 */

int parse(char buffer[1024], char *tokens[512], char *argv[512],
          char *redirect[512]) {
    char *str = buffer;
    char *token;
    // a variable representing the size of argv
    unsigned int i = 0;
    // a variable representing the size of redirect
    unsigned int size_re = 0;

    // tokenize
    while ((token = strtok(str, " \t\n")) != NULL) {
        tokens[i] = token;
        str = NULL;
        i += 1;
    }

    // when there is no user input, and return is hit
    if (tokens[0] == NULL) {
        return 1;
    }

    // put tokens into argv
    for (int j = 0; tokens[j] != NULL; j++) {
        argv[j] = tokens[j];
    }

    // error checkings for cases with nothing after "<", ">", or ">>"
    if (strcmp(argv[i - 1], ">") == 0) {
        fprintf(stderr, "syntax error: no output file\n");
        return 1;
    }

    if (strcmp(argv[i - 1], "<") == 0) {
        fprintf(stderr, "syntax error: no input file\n");
        return 1;
    }

    if (strcmp(argv[i - 1], ">>") == 0) {
        fprintf(stderr, "syntax error: no output file\n");
        return 1;
    }

    // handle the redirections by moving them from argv to redirect
    for (unsigned int k = 0; k < i - 1; k++) {
        if (strcmp(argv[k], ">") == 0 || strcmp(argv[k], "<") == 0 ||
            strcmp(argv[k], ">>") == 0) {
            // adding to redirect
            redirect[size_re] = argv[k];
            redirect[size_re + 1] = argv[k + 1];
            // increasing the total size of redirect by 2
            size_re += 2;
            // removing from argv
            memmove(argv + k, argv + k + 2, 8 * (i - k - 2));
            argv[i - 2] = NULL;
            argv[i - 1] = NULL;
            // decreasing the total size of argv by 2
            i -= 2;
            // k now refers to the next string to be parsed
            k -= 1;
            // handle redirections with no command
            if (i == 0) {
                fprintf(stderr, "error: redirects with no command\n");
                return 1;
            }
        }
    }

    int input_num = 0;
    int output_num = 0;
    // count the number of the same kinds of redirections
    for (unsigned int m = 0; m < size_re; m += 2) {
        if (strcmp(redirect[m], "<") == 0) {
            input_num += 1;
        } else {
            output_num += 1;
        }
    }

    // handle multiple same redirections
    if (input_num >= 2) {
        fprintf(stderr, "syntax error: multiple input files\n");
        return 1;
    } else if (output_num >= 2) {
        fprintf(stderr, "syntax error: multiple output files\n");
        return 1;
    }

    return 0;
}

/*
 * main()
 *
 * - Description: this is the main function where the shell is implemented.
 *
 */

int main() {
    // used for saving the value returned from read
    ssize_t bytesRead;

    while (1) {
#ifdef PROMPT
        if (printf("33sh> ") < 0) {
            fprintf(stderr, "error when printing prompt.\n");
        }
        if (fflush(stdout) < 0) {
            fprintf(stderr, "error when using fflush.\n");
        }
#endif
        // initializing necessary arrays
        char buffer[1024];
        memset(buffer, 0, 1024);

        char *tokens[512];
        memset(tokens, 0, 512 * sizeof(char *));

        char *argv[512];
        memset(argv, 0, 512 * sizeof(char *));

        char *redirect[512];
        memset(redirect, 0, 512 * sizeof(char *));

        char buf[1024];
        memset(buf, 0, 1024);

        // reading from file descripter 0, which stores user input
        if ((bytesRead = read(0, buffer, 1024)) == -1) {
            perror("user input");
            continue;
        } else if (bytesRead == 0) {
            // the case where user inputs nothing and hits Command D
            exit(1);
        } else {
            // handling the error produced by parse()
            if (parse(buffer, tokens, argv, redirect) == 1) {
                continue;
            }
        }

        // command exit
        if (strcmp(argv[0], "exit") == 0) {
            exit(0);
        }

        // child process
        if (fork() == 0) {
            // handling redirection. Note that the maximum size of
            // redirect is 4.
            for (int i = 0; i < 4; i += 2) {
                if (redirect[i] != NULL) {
                    // input file redirection
                    if (strcmp(redirect[i], "<") == 0) {
                        // close fd 0
                        if (close(0) == -1) {
                            perror("close");
                            exit(1);
                        }
                        // set flags to read only
                        int fd_0 = open(redirect[i + 1], O_RDONLY);
                        if (fd_0 == -1) {
                            perror("open");
                            exit(1);
                        }
                    }
                    // output file redirection with ">"
                    else if (strcmp(redirect[i], ">") == 0) {
                        // close fd 1
                        if (close(1) == -1) {
                            perror("close");
                            exit(1);
                        }
                        // set flags to write only, create, and truncate
                        int fd_1 = open(redirect[i + 1],
                                        O_WRONLY | O_CREAT | O_TRUNC, 0666);
                        if (fd_1 == -1) {
                            perror("open");
                            exit(1);
                        }
                    }
                    // output file redirection with ">>"
                    else {
                        // close fd 1
                        if (close(1) == -1) {
                            perror("close");
                            exit(1);
                        }
                        // set flags to write only, create, and append
                        int fd_2 = open(redirect[i + 1],
                                        O_WRONLY | O_CREAT | O_APPEND, 0666);
                        if (fd_2 == -1) {
                            perror("open");
                            exit(1);
                        }
                    }
                }
            }

            // command cd
            if (strcmp(argv[0], "cd") == 0) {
                // no file
                if (argv[1] == NULL) {
                    fprintf(stderr, "cd: syntax error \n");
                } else {
                    // using chdir() system call
                    int cd = chdir(
                        strcat(strcat(getcwd(buf, sizeof(buf)), "/"), argv[1]));
                    if (cd == -1) {
                        perror("cd");
                        exit(1);
                    } else {
                        continue;
                    }
                }
            }

            // command ln
            else if (strcmp(argv[0], "ln") == 0) {
                // no file
                if (argv[1] == NULL || argv[2] == NULL) {
                    fprintf(stderr, "ln: syntax error \n");
                } else {
                    // using link() system call
                    if (link(argv[1], argv[2]) == -1) {
                        perror("ln");
                        exit(1);
                    }
                }
            }

            // command rm
            else if (strcmp(argv[0], "rm") == 0) {
                // no file
                if (argv[1] == NULL) {
                    fprintf(stderr, "rm: syntax error \n");
                } else {
                    // using unlink() system call
                    if (unlink(argv[1]) == -1) {
                        perror("rm");
                        exit(1);
                    }
                }
            } else {
                // handling all other commands using execv() system call
                char *full_name = argv[0];
                // extracting the binary name
                if (strrchr(argv[0], '/') != NULL) {
                    argv[0] = strrchr(argv[0], '/') + 1;
                }
                execv(full_name, argv);
                perror("execv");
                exit(1);
            }
            exit(0);
        }

        // Parent Process continues
        if (wait(0) == -1) {
            perror("wait");
            continue;
        }
    }
    return 0;
}