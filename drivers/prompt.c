#include "prompt.h"
#include "../headers.h"

void prompt() {
    char *a = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
    if(time_taken > 1) {
        sprintf(a, "took %d seconds", time_taken);
    } else {
        a[0] = '\0';
    }
    printf("<" GRN "%s" RESET ":" BLU "%s" RESET " %s> ", hname, get_dir(owd, cwd), a);
    time_taken = 0;
}

char* get_dir(char* owd, char* cwd)
{
    char *p = strstr(cwd, owd);
    if(p == NULL || *p != *cwd)
    {
        return cwd;
    }
    else
    {
        char *q = (char*)malloc(sizeof(char)*(strlen(cwd) - strlen(owd) + 2));
        strcpy(q, "~");
        strcat(q, p+strlen(owd));
        return q;
    }
}