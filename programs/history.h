#ifndef __HISTORY_H__
#define __HISTORY_H__

void load_history(void);
void add_to_history(char *);
void save_history(void);
int history(int, char **);

#endif