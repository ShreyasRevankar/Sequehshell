#include "drivers/prompt.h"
#include "headers.h"
#include "drivers/hostname.h"
#include "drivers/commands.h"
#include "drivers/init.h"
#include "drivers/background.h"
#include "drivers/signals.h"
#include "drivers/raw.h"
#include "drivers/autocomplete.h"

char *owd, *cwd, *prev_cwd;
char *history_stack[20];
int stack_index;
char *hname;
int time_taken;
int num_bgs;
background bgs[MAX_BUFFER_SIZE];
int foreground_pid;
int REAL_STDOUT, REAL_STDIN, REAL_STDERR;
// int fd[2];

int main()
{
    init();
    signal(SIGCHLD,finish_background);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGINT,SIG_IGN);


    // while (1)
    // {
    //     prompt();
    //     char a[MAX_BUFFER_SIZE] = {'\0'};

    //     char ch = getc(stdin);
    //     if(ch == EOF) {
    //         printf("\n");
    //         // tcsetattr(STDIN_FILENO,TCSANOW,&old);
    //         exit(0);
    //     }
    //     int i = 0;
    //     while(ch != '\n') {
    //         a[i++] = ch;
    //         ch = getc(stdin);
    //         // if(ch == '\t') {
    //         //     auto_complete(a);
    //         // }
    //     }
    //     // a[i++] = '\0';

    //     commands(a);
    // }

    char a[MAX_BUFFER_SIZE] = {'\0'};
    char ch;
    char match[MAX_BUFFER_SIZE] = {'\0'};
    bool ac_on = false;
    int num_matches = 0;

    while (1) {
        setbuf(stdout, NULL);
        enableRawMode();
        prompt();
        memset(a, '\0', MAX_BUFFER_SIZE);
        if(ac_on) {
            strcpy(a, match);
            printf("%s", a);
            memset(match, '\0', MAX_BUFFER_SIZE);
            ac_on = false;
        }
        int pt = strlen(a);
        while (read(STDIN_FILENO, &ch, 1) == 1) {
            if (iscntrl(ch)) {
                if (ch == 10) break; // Enter
                // else if (ch == 27) {
                //     char buf[3];
                //     buf[2] = 0;
                //     if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
                //         printf("\rarrow key: %s", buf);
                //     }
                // }
                else if (ch == 127) { // backspace
                    if (pt > 0) {
                        if (a[pt-1] == 9) {
                            for (int i = 0; i < 7; i++) {
                                printf("\b");
                            }
                        }
                        a[--pt] = '\0';
                        printf("\b \b");
                    }
                } else if (ch == 9) { // TAB character
                    num_matches = auto_complete(a, match);
                    if(num_matches == 1) {
                        for(int i = 0; i < strlen(a); i++) {
                            printf("\b \b");
                        }
                        printf("%s", match);
                        pt = strlen(match);
                        strcpy(a, match);
                    } else {
                        ac_on = true;
                        break;
                    }
                } else if (ch == 4) { // ctrl + d
                    printf("\n");
                    exit(0);
                } else {
                    printf("%d\n", ch);
                }
            } else {
                a[pt++] = ch;
                printf("%c", ch);
            }
        }
        disableRawMode();

        // printf("\nInput Read: [%s]\n", a);
        printf("\n");
        if(!ac_on) commands(a);
    }
}
