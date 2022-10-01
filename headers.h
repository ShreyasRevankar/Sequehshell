#ifndef __HEADERS_H__
#define __HEADERS_H__

#define MAX_BUFFER_SIZE 1024

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <grp.h>
#include <dirent.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <termios.h>
#include "drivers/background.h"


extern char *owd, *cwd, *prev_cwd;
extern char *history_stack[20];
extern int stack_index;
extern char *hname;
extern int time_taken;
extern int num_bgs;
extern background bgs[MAX_BUFFER_SIZE];
extern int foreground_pid;
// extern int fd[2];
extern int REAL_STDIN, REAL_STDOUT, REAL_STDERR;


#endif // __HEADERS_H__