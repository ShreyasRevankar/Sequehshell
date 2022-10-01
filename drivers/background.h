#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

typedef struct background background;

struct background {
    int pid;
    char **command;
    int size;
};

void init_bg(background *bg, int size);
int add_background(int pid, char **command, int size);
int remove_background(int pid);
char* get_process_name(int pid);
void finish_background();

#endif /* __BACKGROUND_H__ */