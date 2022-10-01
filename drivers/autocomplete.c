#include "../headers.h"

int auto_complete(char* a, char *to_return) {
    char* a_copy = malloc(sizeof(char) * strlen(a));
    strcpy(to_return, "");
    strcpy(a_copy, a);
    DIR *dir = opendir(".");
    // if(dir == NULL) {
    //     printf("%s: No such file or directory\n", directory);
    //     return 1;
    // }
    struct dirent *dirent;
    // save the d_name in an array
    char** files = (char**)malloc(sizeof(char*) * MAX_BUFFER_SIZE);
    int i = 0;
    while((dirent = readdir(dir)) != NULL) {
        if(dirent->d_name[0] == '.') {
            continue;
        }
        files[i] = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
        strcpy(files[i], dirent->d_name);
        if(dirent->d_type == DT_DIR) {
            strcat(files[i], "/");
        }
        i++;
    }

    char* token = strtok(a_copy, " ");
    char* initial = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
    if(token == NULL) {
        strcpy(initial, "");
    } else {
        strcpy(initial, token);
    }
    while(token != NULL) {
        token = strtok(NULL, " ");
        if(token != NULL) {
            strcat(to_return, initial);
            strcat(to_return, " ");
            strcpy(initial, token);
        }
    }

    // char** matches = (char**)malloc(sizeof(char*) * MAX_BUFFER_SIZE);
    int num_matches = 0;
    char* match = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
    strcpy(match, "");
    for(int j = 0; j < i; j++) {
        if(strncmp(files[j], initial, strlen(initial)) == 0) {
            if(num_matches == 0) {
                strcpy(match, files[j]);
                num_matches++;
            } else {
                if(num_matches == 1) {
                    printf("\n%s\n", match);
                }
                int k = 0;
                while(match[k] != '\0' && files[j][k] != '\0' && match[k] == files[j][k]) {
                    k++;
                }
                match[k] = '\0';
                printf("%s\n", files[j]);
                num_matches++;
            }
        }
    }
    strcat(to_return, match);
    if(num_matches == 1 && match[strlen(match)-1] != '/') {
        strcat(to_return, " ");
    }
    // if(num_matches == 1)
    //     printf("%s", to_return + strlen(a));


    for(int j = 0; j < i; j++) {
        free(files[j]);
    }
    free(files);
    closedir(dir);
    return num_matches;
}