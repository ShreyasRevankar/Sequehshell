#include "../headers.h"

void ctrl_c() {
    if(foreground_pid != -1) {
        printf("%d\n", foreground_pid);
        kill(foreground_pid, SIGINT);
        foreground_pid = -1;
        printf("\n");
    }
}

void ctrl_z() {
    if(foreground_pid != -1) {
        kill(foreground_pid, SIGTSTP);
        foreground_pid = -1;
        // add_background(foreground_pid, "fg");
    }
}