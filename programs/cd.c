#include "../headers.h"

int cd(int argn, char** args) {
    // for(int i = 0; i < argn; i++) {
    //     printf("%s\n", args[i]);
    //     fflush(stdout);
    // }
    if(argn > 2) {
        printf("cd: too many arguments\n");
        return 1;
    }
    if(argn == 1 || strcmp(args[1], "~") == 0) {
        chdir(owd);
        strcpy(prev_cwd, cwd);
        strcpy(cwd, owd);
        return 0;
    }
    if(strcmp(args[1], "-") == 0) {
        chdir(prev_cwd);
        printf("%s\n", prev_cwd);
        char* temp = (char*)malloc(sizeof(char) * 1024);
        strcpy(temp, cwd);
        strcpy(cwd, prev_cwd);
        strcpy(prev_cwd, temp);
        free(temp);
        return 0;
    }
    if(strlen(args[1]) > 1 && args[1][0] == '~') {
        char* temp = (char*)malloc(sizeof(char) * 1024);
        strcpy(temp, owd);
        strcat(temp, args[1] + 1);
        strcpy(args[1], temp);
        free(temp);
    }
    if(chdir(args[1]) == -1) {
        printf("cd: %s: No such file or directory\n", args[1]);
        return 1;
    } else {
        strcpy(prev_cwd, cwd);
        strcpy(cwd, getcwd(NULL, 0));
        return 0;
    }
}