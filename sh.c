#include <stdio.h>

char *find_binary_name(char *argv[512]){
    char *strr;
    strr = argv[0];
    char *s; 
    s = "";

    if (strr == NULL){
        return strr;
    }
    for (int j = strlen(strr)-1; j >=0; j--){

        if (strr[j] == '/'){
            if (j == strlen(strr)-1){
                return s;
            }
            return strstr(strr, &strr[j+1]);
        }

    }
    return strr;
}

void parse(char buffer[1024], char *tokens[512], char *argv[512]) {
    // TODO: write your code here
    char *str = buffer;
    char *token;
    int i = 0;
    while ((token = strtok(str, " \t\n")) != NULL){
        tokens[i] = token;
        argv[i] = token;
        str = NULL;
        i++;
    }

    argv[0] = find_binary_name(argv);

    argv[i] = NULL;

    // You may create helper functions if you like
}

int main() {
    /* TODO: everything! */
    printf("helloworld");

    return 0;
}
