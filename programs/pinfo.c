#include "../headers.h"

int pinfo(int k, char** args) {
    if(k > 2) {
        printf("pinfo: too many arguments\n");
        return 1;
    }
    pid_t pid = getpid();
    if(k == 2) {
        for(int i = 0; i < strlen(args[1]); i++) {
            if(args[1][i] < '0' || args[1][i] > '9') {
                printf("pinfo: invalid pid\n");
                return 1;
            }
        }
        pid = atoi(args[1]);
    }
    // assert(0);
    char statfile[100];
    sprintf(statfile, "/proc/%d/stat", (int)pid);

    char stats[MAX_BUFFER_SIZE] = {'\0'};

    FILE* f_statfile;
    f_statfile = fopen(statfile, "r");
    if(f_statfile == NULL) {
        perror("fopen");
        return 1;
    }
    fread(stats, MAX_BUFFER_SIZE, 1, f_statfile);
    fclose(f_statfile);
    
    char *stat_array[100] = {NULL};
    stat_array[0] = strtok(stats, " ");
    for (int i = 1; stat_array[i - 1] != NULL; i++)
        stat_array[i] = strtok(NULL, " ");

    
    printf("pid : %d\n", (int)pid);
    printf("process Status : %s%s\n", stat_array[2], strcmp(stat_array[0], stat_array[7]) ? "+" : "");
    printf("memory : %s {Virtual Memory}\n",stat_array[22]);

    sprintf(statfile, "/proc/%d/exe", (int)pid);
    char* execPath = (char *)malloc(sizeof(char) * MAX_BUFFER_SIZE);
    // memset(execPath, 0, MAX_BUFFER_SIZE - 1);
    int readStat = readlink(statfile, execPath, MAX_BUFFER_SIZE - 1);
    char path[MAX_BUFFER_SIZE];
    strcpy(path, execPath);
    if(readStat != -1) {
        // printf("Executable Path -- %s\n", execPath);
        // printf("owd: %s\n", owd);
        if(strncmp(execPath, owd, strlen(owd)) == 0) {
            strcpy(path, "~");
            strcat(path, execPath + strlen(owd));
        }
    }
    printf("Executable Path : %s\n", readStat == -1 ? "No access" : path);
    free(execPath);
    return 0;
}