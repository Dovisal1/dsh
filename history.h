#ifndef HISTORY_H
#define HISTORY_H

void add_hist_entry(char *line);
char *get_hist_entry(int n);
void list_hist(int n);
void hist_init();
void hist_clean();

#endif
