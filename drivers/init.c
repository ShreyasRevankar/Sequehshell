#include "../headers.h"
#include "hostname.h"
#include "../programs/history.h"
#include "background.h"

void init() {
    hname = hostname();
    owd = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
    cwd = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
    prev_cwd = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
    strcpy(owd, getcwd(NULL, 0));
    strcpy(cwd, getcwd(NULL, 0));
    strcpy(prev_cwd, getcwd(NULL, 0));
    load_history();
    time_taken = 0;
    num_bgs = 0;
    init_bg(bgs, MAX_BUFFER_SIZE);
    foreground_pid = -1;
    REAL_STDIN = dup(0);
    REAL_STDOUT = dup(1);
    REAL_STDERR = dup(2);
    // system("clear");
}