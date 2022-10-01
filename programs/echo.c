#include "../headers.h"

int echo(int argn, char** args) {
    for(int i = 1; i < argn; i++) {
        printf("%s ", args[i]);
    }
    printf("\n");
    return 0;
}