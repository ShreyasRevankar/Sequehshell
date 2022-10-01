#ifndef __COMMANDS_H__
#define __COMMANDS_H__

void commands(char *);
void get_commands(char*, char***, char***, int[]);
void process_commands(int, char**);
int execute_commands(int, char**, int);
// void run_background(char**, int);
// int run_foreground(char**, int);
int run(char**, int, int);
int run_command(char**, int);
void pipeline(char*);
void execute(char*);
void do_pipe(char*, int);

#endif // __COMMANDS_H__