#include "../headers.h"

void sig(int k, char** args) {
    if(k > 3) {
        printf("sig: too many arguments\n");
        return;
    }
    if(k < 3) {
        printf("sig: too few arguments\n");
        return;
    }
    int pid = bgs[atoi(args[1])].pid;
    if(pid == -1) {
        printf("sig: invalid job\n");
        return;
    }
    int sig_n = atoi(args[2]);
    if(sig_n < 1 || sig_n > 64) {
        printf("sig: invalid signal number\n");
        return;
    }
    kill(pid, sig_n);
}