#include "../headers.h"
#include "prompt.h"

void init_bg(background *bg, int size) {
    for (int i = 0; i < size; i++) {
        bg[i].pid = -1;
        bg[i].command = NULL;
    }
}

int add_background(int pid, char **name, int size) {
    for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
        if (bgs[i].pid == -1) {
            bgs[i].pid = pid;
            bgs[i].command = (char**)malloc(sizeof(char*) * size);
            for (int j = 0; j < size; j++) {
                bgs[i].command[j] = (char*)malloc(sizeof(char) * strlen(name[j]));
                strcpy(bgs[i].command[j], name[j]);
            }
            bgs[i].size = size;
            num_bgs++;
            return 0;
        }
    }
    return -1;
}

int remove_background(int pid) {
    for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
        if (bgs[i].pid == pid) {
            bgs[i].pid = -1;
            free(bgs[i].command);
            bgs[i].command = NULL;
            num_bgs--;
            return 0;
        }
    }
    return -1;
}

char* get_process_name(int pid) {
    for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
        if (bgs[i].pid == pid) {
            return bgs[i].command[0];
        }
    }
    return NULL;
}

void finish_background(){
    bool flag = false;
    pid_t pid;
    int status;
    printf("\n");
    while((pid  = waitpid(-1, &status, WNOHANG | WUNTRACED))>0) {
        for(int x=0; x < MAX_BUFFER_SIZE; x++) {
            if(pid == bgs[x].pid) {
                assert(0);
                flag = true;
                if (WIFEXITED(status)){
                    fprintf(stderr,"%s with pid %d exited normally\n", get_process_name(pid), pid);
                }
                else if (WIFSTOPPED(status)){
                    fprintf(stderr,"%s with pid %d suspended normally\n", get_process_name(pid), pid);
                }
                else {
                    fprintf(stderr,"%s with pid %d exited abnormally\n", get_process_name(pid), pid);
                }
                if(remove_background(pid) == -1) {
                    fprintf(stderr,"Couldn't remove process\n");
                }
            }
        } 
        fflush(stderr);
    }
    if(flag) prompt();   
    fflush(stdout);
   
    return;
}
