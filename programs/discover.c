#include "../headers.h"
#include "discover.h"

int discover(int k, char** args) {
    int dir_or_file = 0;
    char *target_dir = ".";
    char *file = NULL;
    if(k > 1) {
        for(int i = 1; i < k; i++) {
            if(strcmp(args[i], "-d") == 0) {
                dir_or_file--;
            } else if(strcmp(args[i], "-f") == 0) {
                dir_or_file++;
            } else if(args[i][0] == '"') {
                file = args[i] + 1;
                file[strlen(file) - 1] = '\0';
            } else {
                target_dir = args[i];
            }
        }
    }
    if(target_dir[0] == '~') {
        char *temp = malloc(sizeof(char) * MAX_BUFFER_SIZE);
        strcpy(temp, owd);
        strcat(temp, target_dir + 1);
        strcpy(target_dir, temp);
        free(temp);
    }
    // printf("%s\n", target_dir);
    DIR *dir = opendir(target_dir);
    if(dir == NULL) {
        printf("%s: No such file or directory\n", target_dir);
        return 1;
    }
    if(!file && dir_or_file != 1) {
        printf("%s\n", target_dir);
    }
    return explore(target_dir, dir_or_file, file);
}

int explore(char* target_dir, int dir_or_file, char* file) {
    DIR *dir = opendir(target_dir);
    if(dir == NULL) {
        printf("%s: No such file or directory\n", target_dir);
        return 1;
    }
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL) {
        if(entry->d_name[0] == '.') {
            continue;
        }
        if(file && strcmp(entry->d_name, file) == 0) {
            if(dir_or_file == 0 || (dir_or_file == -1 && entry->d_type == DT_DIR) || (dir_or_file == 1 && entry->d_type != DT_DIR)) {
                printf("%s/%s\n", target_dir, entry->d_name);
            }
        }
        if(!file && (dir_or_file == 0 || (dir_or_file == -1 && entry->d_type == DT_DIR) || (dir_or_file == 1 && entry->d_type != DT_DIR))) {
            printf("%s/%s\n", target_dir, entry->d_name);
        }
        if(entry->d_type == DT_DIR) {
            char *new_target_dir = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
            strcpy(new_target_dir, target_dir);
            if(target_dir[strlen(target_dir) - 1] != '/') strcat(new_target_dir, "/");
            strcat(new_target_dir, entry->d_name);
            explore(new_target_dir, dir_or_file, file);
            free(new_target_dir);
        }
    }
    closedir(dir);
}