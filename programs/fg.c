#include "../headers.h"
#include "../drivers/background.h"

void fg(int k, char** args) {
    if(k > 2) {
        printf("fg: too many arguments\n");
        return;
    }
    if(k < 2) {
        printf("fg: too few arguments\n");
        return;
    }
    int pid = bgs[atoi(args[1])].pid;
    if(pid == -1) {
        printf("fg: invalid job\n");
        return;
    }
    kill(pid, SIGCONT);
    remove_background(pid);
    foreground_pid = pid;
    int status;

    clock_t start, end;
    start = time(NULL);

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(STDIN_FILENO, pid);
    waitpid(pid, &status, WUNTRACED);
    end = time(NULL);
    time_taken += end - start;
    if(WIFSTOPPED(status)) {
        add_background(pid, args, k);
    }
    tcsetpgrp(STDIN_FILENO, getpid());
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
}