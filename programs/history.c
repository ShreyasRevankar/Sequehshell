#include "../headers.h"
#include "history.h"

void load_history() {
    for(int i = 0; i < 20; i++) {
        history_stack[i] = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
    }
    FILE *fp = fopen(".history", "r");
    if(fp == NULL) {
        return;
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while((read = getline(&line, &len, fp)) != -1) {
        if(strlen(line) > 1) {
            line[strlen(line) - 1] = '\0';
            add_to_history(line);
        }
    }
    fclose(fp);
}

void add_to_history(char *a) {
    if(stack_index > 0 && strcmp(a, history_stack[stack_index-1]) == 0) return;
    // printf("%s %s %d\n", a, history_stack[stack_index], strcmp(a, history_stack[stack_index]));

    if(stack_index == 20) {
        for(int i = 1; i < 20; i++) {
            strcpy(history_stack[i - 1], history_stack[i]);
        }
        stack_index--;
    }
    strcpy(history_stack[stack_index], a);
    stack_index++;
    // history();
    save_history();
}

void save_history() {
    char *filename = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
    strcpy(filename, owd);
    strcat(filename, "/.history");
    FILE *fp = fopen(filename, "w");
    for(int i = 0; i < stack_index; i++) {
        fprintf(fp, "%s\n", history_stack[i]);
    }
    fclose(fp);
}

int history(int k, char** args) {
    if(k > 2) {
        printf("history: Too many arguments\n");
        return 1;
    }

    int num = 10;
    if(k == 2) {
        for(int i = 0; i < strlen(args[1]); i++) {
            if(!isdigit(args[1][i])) {
                printf("history: numeric argument required\n");
                return 1;
            }
        }
        num = atoi(args[1]);
    }
    if(num > 20) {
        printf("history: argument out of range\n");
        return 1;
    }
    // printf("boobs\n");
    num = stack_index < num ? stack_index : num;
    // printf("%d %d", stack_index, num);
    // for(int i = 0; i < stack_index; i++) {
    //     printf("- %s", history_stack[i]);
    // }
    // add_to_history();
    for(int i = stack_index - num; i < stack_index; i++) {
        printf("%s\n", history_stack[i]);
    }
    return 0;
}