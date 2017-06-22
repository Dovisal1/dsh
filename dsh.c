/* \file dsh.c
 *
 * A simple shell
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "history.h"

#define PROMPT "dsh> "
#define WHITESPACE " \t\r\n\v"
#define SYMBOLS
#define DELIMS WHITESPACE SYMBOLS

struct cmd {
	int argc;
	char **argv;
	char *base;
};

int getcmd(char *buf, int sz)
{
	printf(PROMPT);
	fflush(stdout);
	if (fgets(buf, sz, stdin) == NULL)
		return 0;
	return 1;
}

/* exec, never return */

void die(int status, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt,ap);
	va_end(ap);

	exit(status);
}

pid_t dfork()
{
	pid_t pid = fork();
	if(pid == -1)
		die(-1, "fork error");
	return pid;
}

struct cmd *parseline(char *line)
{
	struct cmd *cmd = malloc(sizeof(struct cmd));
	line = strdup(line);
	*cmd = (struct cmd){.base=line};

	char *scratch = NULL;
	char *txt = strtok_r(line, DELIMS, &scratch);

	if (!txt) {
		free(cmd);
		return NULL;
	}

	while (txt) {
		cmd->argv = realloc(cmd->argv, sizeof(char*) * ++(cmd->argc));
		cmd->argv[cmd->argc-1] = txt;
		txt = strtok_r(NULL, DELIMS, &scratch);
	}

	return cmd;
}

void free_cmd(struct cmd *cmd)
{
	if (!cmd)
		return;
	free (cmd->base);
	free (cmd->argv);
	free (cmd);
}

void run_file(struct cmd *cmd)
{
	switch(dfork()) {
	case 0:
		execvp(cmd->argv[0], cmd->argv);
		/* if error */
		perror(cmd->argv[0]);
		exit(1);
		break;
	default:
		wait(NULL);
		break;
	}

}

void run_hist(struct cmd *cmd)
{
	if (cmd->argc > 2)
		fprintf(stderr, "dsh: history: too many args\n");

	char *h = cmd->argc == 1 ? get_hist_entry(0) : get_hist_entry(atoi(cmd->argv[1]));

	struct cmd *hist_cmd = parseline(h);
	run_file(hist_cmd);
}



int main()
{
	hist_init();
	char line[4096];

	while(getcmd(line, sizeof(line))) {
		add_hist_entry(line);
		struct cmd *cmd = parseline(line);

		if (!cmd)
			continue;

		if (!strcmp(cmd->argv[0], "history"))
			list_hist(cmd->argc == 1 ? 0 : atoi(cmd->argv[1]));
		else
			run_file(cmd);

		free_cmd(cmd);
	}
	hist_clean();
	printf("\n");
}
