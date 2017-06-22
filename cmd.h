#ifndef CMD_H
#define CMD_H

struct cmd {
	int argc;
	char **argv;
	char *base;
	char *cl;
};

int getcmd(char *buf, int sz);
struct cmd *parseline(char *line);
void free_cmd(struct cmd *cmd);

#endif
