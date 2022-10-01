#include "../headers.h"

int cmp(const void* a, const void* b) {
    return strcmp(bgs[(int)*(int*)a].command[0], bgs[(int)*(int*)b].command[0]);
}

void jobs(int k, char** args) {
    int r_s = 0;
    for(int i = 1; i < k; i++) {
        if(strcmp(args[i], "-r") == 0) {
            r_s--;
        } else if(strcmp(args[i], "-s") == 0) {
            r_s++;
        } else {
            printf("jobs: invalid option\n");
            return;
        }
    }
    int cur_jobs[num_bgs];
    int j = 0;
    for(int i = 0; i < MAX_BUFFER_SIZE; i++) {
        if(bgs[i].pid != -1) {
            cur_jobs[j++] = i;
        }
    }
    qsort(cur_jobs, j, sizeof(int), cmp);
    for(int i = 0; i < j; i++) {
        int pid = bgs[cur_jobs[i]].pid;

        char statfile[100];
        sprintf(statfile, "/proc/%d/stat", (int)pid);

        char stats[MAX_BUFFER_SIZE] = {'\0'};

        FILE* f_statfile;
        f_statfile = fopen(statfile, "r");
        if(f_statfile == NULL) {
            perror("fopen");
            return;
        }
        fread(stats, MAX_BUFFER_SIZE, 1, f_statfile);
        fclose(f_statfile);
        
        char *stat_array[100] = {NULL};
        stat_array[0] = strtok(stats, " ");
        for (int i = 1; stat_array[i - 1] != NULL; i++)
            stat_array[i] = strtok(NULL, " ");

        if(r_s == 0 || (r_s == 1 && strncmp(stat_array[2], "T", 1) == 0) || (r_s == -1 && strncmp(stat_array[2], "T", 1) != 0)) {
            printf("[%d] %s ", cur_jobs[i], stat_array[2][0] == 'T' ? "Stopped" : "Running");
            for(int k = 0; k < bgs[cur_jobs[i]].size; k++) {
                printf("%s ", bgs[cur_jobs[i]].command[k]);
            }
            printf("[%d]\n", bgs[cur_jobs[i]].pid);
        }
    }
}