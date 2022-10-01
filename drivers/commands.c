#include "../headers.h"
#include "commands.h"
#include "../programs/pwd.h"
#include "../programs/cd.h"
#include "../programs/echo.h"
#include "../programs/ls.h"
#include "../programs/history.h"
#include "../programs/exit.h"
#include "../programs/pinfo.h"
#include "../programs/discover.h"
#include "../programs/sig.h"
#include "../programs/jobs.h"
#include "../programs/fg.h"
#include "../programs/bg.h"
#include "background.h"

void commands(char *a) {
    int num_commands[2];
    char **fore, **back;
    if(strlen(a) == 1) {
        return;
    } else {
        char *a_copy = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
        strcpy(a_copy, a);
        if(strtok(a_copy, " \t") == NULL) return;
        add_to_history(a);
        // pipeline(a);
        // dup2(REAL_STDOUT, 1);
        // dup2(REAL_STDIN, 0);
        execute(a);
    // get_commands(a, &fore, &back, num_commands);
    // // for(int i = 0; i < num_commands[0]; i++) {
    // //     printf("%d %s %d\n", i, fore[i], strlen(fore[i]));
    // // }
    // // for(int i = 0; i < num_commands[1]; i++) {
    // //     printf("%d %s %d\n", i, back[i], strlen(back[i]));
    // // }
    // // fflush(stdout);
    // process_commands(num_commands[0], fore);
    // process_commands(num_commands[1], back);
    // execute_commands(num_commands[1], back, 0);
    // execute_commands(num_commands[0], fore, 1);
    }
}

void execute(char* a) {
    int num_commands[2];
    char **fore, **back;
    get_commands(a, &fore, &back, num_commands);
    // for(int i = 0; i < num_commands[0]; i++) {
    //     printf("%d %s %d\n", i, fore[i], strlen(fore[i]));
    // }
    // for(int i = 0; i < num_commands[1]; i++) {
    //     printf("%d %s %d\n", i, back[i], strlen(back[i]));
    // }
    // fflush(stdout);
    process_commands(num_commands[0], fore);
    process_commands(num_commands[1], back);
    execute_commands(num_commands[1], back, 0);
    execute_commands(num_commands[0], fore, 1);
}

void pipeline(char* a) {
    // printf("%s\n", a);
    char* token, *subtoken;
    char* saveptr1, *saveptr2;
    char delim[2] = "|";
    char delim2[2] = " ";
    char a_copy[MAX_BUFFER_SIZE];
    strcpy(a_copy, a);
    int num_pipes = 0;
    for(int i = 0; i < strlen(a); i++) {
        if(a[i] == '|') {
            num_pipes++;
        }
    }
    if(num_pipes == 0) {
        execute(a);
        return;
    }
    token = strtok_r(a, delim, &saveptr1);
    int len = 0;
    while(token != NULL) {
        // subtoken = strtok_r(token, delim2, &saveptr2);
        // while(subtoken != NULL) {
        //     subtoken = strtok_r(NULL, delim2, &saveptr2);
        // }
        token = strtok_r(NULL, delim, &saveptr1);
        len++;
    }
    char** commands = (char**)malloc(sizeof(char*) * len);
    strcpy(a, a_copy);
    // printf("%s\n", a);
    // assert(0);
    token = strtok_r(a, delim, &saveptr1);
    int i = 0;
    while(token != NULL) {
        commands[i] = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
        strcpy(commands[i], token);
        token = strtok_r(NULL, delim, &saveptr1);
        i++;
    }
    int fd[2];
    int fd_in = 0;
    int stdout_copy = dup(1);
    // printf("len: %d", len);
    for(int i = 0; i < len; i++) {
        pipe(fd);
        int pid = fork();
        if(pid == 0) {
            dup2(fd_in, 0);
            if(i != len - 1) dup2(fd[1], 1);
            else dup2(stdout_copy, 1);
            close(fd[0]);
            execute(commands[i]);
            exit(0);
        } else {
            int status;
            waitpid(pid, &status, 0);
            if(i != len -1) close(fd[1]);
            fd_in = fd[0];
        }
    }
}


void get_commands(char* a, char*** p_fore, char*** p_back, int num[]) {
    int num_back = 0, num_fore = 0;
    char** fore = (char**)malloc(sizeof(char*) * MAX_BUFFER_SIZE);
    char** back = (char**)malloc(sizeof(char*) * MAX_BUFFER_SIZE);
    char* token, *subtoken;
    char* saveptr1, *saveptr2;
    char delim[2] = ";";
    char delim2[2] = "&";
    char a_copy[MAX_BUFFER_SIZE];
    strcpy(a_copy, a);

    for(int i = 0; i < strlen(a); i++) {
        if(a[i] == ';') {
            num_fore++;
        } else if(a[i] == '&') {
            num_back++;
        }
    }
    if(a[strlen(a)-1] != ';' && a[strlen(a)-1] != '&') {
        num_fore++;
    }

    token = strtok_r(a, delim, &saveptr1);
    int len = 0;
    int f = 0, b = 0;

    while(token != NULL) {
        subtoken = strtok_r(token, delim2, &saveptr2);
        while(subtoken != NULL) {
            len += strlen(subtoken);
            if(len < strlen(a_copy) && a_copy[len] == '&') {
                back[b] = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
                strcpy(back[b], subtoken);
                b++;
            } else {
                fore[f] = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
                strcpy(fore[f], subtoken);
                f++;
            }
            subtoken = strtok_r(NULL, delim2, &saveptr2);
            len++;
        }
        token = strtok_r(NULL, delim, &saveptr1);
    }
    *p_fore = fore;
    *p_back = back;
    num[0] = f;
    num[1] = b;
}

void process_commands(int n, char** commands) {
    // remove extra spaces
    for (int i = 0; i < n; i++) {
        // remove initial spaces
        while (commands[i][0] == ' ' || commands[i][0] == '\t') {
            for (int j = 0; j < strlen(commands[i]); j++) {
                commands[i][j] = commands[i][j+1];
            }
        }
        int k = strlen(commands[i]);
        for(int j = 0; j < k-1; j++) {
            if (commands[i][j] == ' ' && commands[i][j+1] == ' ') {
                for (int l = j+1; l < k; l++) {
                    commands[i][l] = commands[i][l+1];
                }
                k--;
                j--;
            }
        }
    }
}

int execute_commands(int num_commands, char** commands, int is_fore) {
    int to_return = 0;

    for(int i = 0; i < num_commands; i++) {
        if(strlen(commands[i]) == 0) {
            continue;
        }
        int args = 0;
        for (int j = 0; j < strlen(commands[i]); j++) {
            if (commands[i][j] == ' ') {
                args++;
            }
        }
        char** args_arr = (char**)calloc(args+1, sizeof(char*));
        char** argument_copy = (char**)calloc(args+1, sizeof(char*));
        int w = 0;
        int k = 0;
        char* p = strtok(commands[i], " \t");
        while (p != NULL)
        {
            args_arr[k] = p;
            k++;
            p = strtok(NULL, " \t");
        }
        int stdin_copy = dup(0);
        int stdout_copy = dup(1);
        int input_fd = -1, output_fd = -1;

        int error_occurred = 0;

        for(int j = 0; j < k; j++) {
            if(strcmp(args_arr[j], "<") == 0) {
                if(j == k-1) {
                    printf("Error: no input file specified");
                    error_occurred = 1;
                    break;
                }
                input_fd = open(args_arr[j+1], O_RDONLY);
                if(input_fd == -1) {
                    perror("open");
                    error_occurred = 1;
                    break;
                }
                dup2(input_fd, 0);
                j++;
                // close(input_fd);
                // for(int l = j; l < k-2; l++) {
                //     args_arr[l] = args_arr[l+2];
                // }
                // k -= 2;
                // args_arr[k] = NULL;
            } else if(strcmp(args_arr[j], ">") == 0) {
                if(j == k-1) {
                    printf("Error: no output file specified");
                    error_occurred = 1;
                    break;
                }
                output_fd = open(args_arr[j+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if(output_fd == -1) {
                    perror("open");
                    error_occurred = 1;
                    break;
                }
                dup2(output_fd, 1);
                j++;
                // close(output_fd);
                // for(int l = j; l < k-2; l++) {
                //     args_arr[l] = args_arr[l+2];
                // }
                // k -= 2;
                // args_arr[k] = NULL;
            } else if(strcmp(args_arr[j], ">>") == 0) {
                if(j == k-1) {
                    printf("Error: no output file specified");
                    error_occurred = 1;
                    break;
                }
                output_fd = open(args_arr[j+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                if(output_fd == -1) {
                    perror("open");
                    error_occurred = 1;
                    break;
                }
                dup2(output_fd, 1);
                j++;
                // close(output_fd);
                // for(int l = j; l < k-2; l++) {
                //     args_arr[l] = args_arr[l+2];
                // }
                // k -= 2;
                // args_arr[k] = NULL;
            } else if(strcmp(args_arr[j], "|") == 0) {
                int fd[2];
                pipe(fd);
                dup2(fd[1], STDOUT_FILENO);
                argument_copy[w] = NULL;
                if(run_command(argument_copy, w) == -1) {
                    run(argument_copy, w, is_fore);
                }
                close(fd[1]);
                dup2(fd[0], STDIN_FILENO);
                w = 0;
                memset(argument_copy, 0, args+1);
                dup2(stdout_copy, STDOUT_FILENO);
                dup2(REAL_STDOUT, STDOUT_FILENO);
                close(fd[0]);
                // for(int l = j; l < k-2; l++) {
                //     args_arr[l] = args_arr[l+2];
                // }
                // k -= 2;
                // args_arr[k] = NULL;
            } else {
                argument_copy[w] = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
                strcpy(argument_copy[w], args_arr[j]);
                w++;
            }
        }
        if(error_occurred) {
            dup2(stdin_copy, 0);
            dup2(stdout_copy, 1);
            continue;
        }
        int r = run_command(argument_copy, w);
        if(r == -1) to_return += run(argument_copy, w, is_fore);
        else to_return += r;
        dup2(stdin_copy, 0);
        dup2(stdout_copy, 1);
    }
    // printf("%d\n", to_return);/
    return to_return;
}


int run(char** args_arr, int k, int fore) {
    pid_t pid;
    int status;
    pid = fork();
    if (pid != 0) {
        if(fore) {
            foreground_pid = pid;
            clock_t start, end;
            start = time(NULL);

            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(STDIN_FILENO, pid);
            waitpid(pid, &status, WUNTRACED);
            end = time(NULL);
            time_taken += end - start;
            if(WIFSTOPPED(status)) {
                add_background(pid, args_arr, k);
            }
            tcsetpgrp(STDIN_FILENO, getpid());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
            return status;
        } else {
            printf("[%d] %d\n", num_bgs, pid);
            if(add_background(pid, args_arr, k) == -1) {
                printf("Error: too many background processes\n");
                kill(pid, SIGKILL);
            }
        }
    } else if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else {
        signal(SIGINT,SIG_DFL);
        signal(SIGTSTP,SIG_DFL);
        
        setpgid(0,0);
        // int to_return;
        // to_return = run_command(args_arr, k);
        // exit(to_return);
        args_arr[k] = NULL;
        if(execvp(args_arr[0], args_arr) == -1) {
            printf("%s: command not found\n", args_arr[0]);
            exit(EXIT_FAILURE);
        }
    }
}

int run_command(char** args_arr, int k) {
    int to_return;
    if (strcmp(args_arr[0], "pwd") == 0) {
        return pwd();
    } else if (strcmp(args_arr[0], "echo") == 0) {
        return echo(k, args_arr);
    } else if (strcmp(args_arr[0], "cd") == 0) {
        return cd(k, args_arr);
    } else if(strcmp(args_arr[0], "ls") == 0) {
        return ls(k, args_arr);
    } else if(strcmp(args_arr[0], "history") == 0) {
        return history(k, args_arr);
    } else if(strcmp(args_arr[0], "exit") == 0) {
        exit_shell();
    } else if(strcmp(args_arr[0], "pinfo") == 0) {
        pinfo(k, args_arr);
    } else if(strcmp(args_arr[0], "discover") == 0) {
        discover(k, args_arr);
    } else if(strcmp(args_arr[0], "sig") == 0) {
        sig(k, args_arr);
    } else if(strcmp(args_arr[0], "jobs") == 0) {
        jobs(k, args_arr);
    } else if(strcmp(args_arr[0], "fg") == 0) {
        fg(k, args_arr);
    } else if(strcmp(args_arr[0], "bg") == 0) {
        bg(k, args_arr);
    } 
    else {
        return -1;
    }

}