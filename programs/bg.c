#include "../headers.h"

void bg(int k, char** args) {
    if(k > 2) {
        printf("bg: too many arguments\n");
        return;
    }
    if(k < 2) {
        printf("bg: too few arguments\n");
        return;
    }
    int pid = bgs[atoi(args[1])].pid;
    if(pid == -1) {
        printf("bg: invalid job\n");
        return;
    }
    kill(pid, SIGCONT);
}