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

#include "builtin.h"
#include "history.h"
#include "cmd.h"

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

void run_builtin(struct cmd *cmd)
{
	switch(builtin_num(cmd)) {
	case 0: /* echo */
		run_echo(cmd);
		break;
	case 1:
		run_cd(cmd);
		break;
	case 2:
		run_history(cmd);
		break;
	default:
		fprintf(stderr, "dsh: builtin panic\n");
	}
}

void run_recall(struct cmd *cmd);

void runcmd(struct cmd *cmd)
{
	if (cmd->argv[0][0] == '!')
		run_recall(cmd);
	else {
		add_hist_entry(cmd->cl);
		if (is_builtin(cmd))
			run_builtin(cmd);
		else
			run_file(cmd);
	}
}

void run_recall(struct cmd *cmd)
{
	char *argv0 = cmd->argv[0];
	argv0++; /* we know *argv0=='!' */

	int hist_item;

	if(*argv0 == '!') /* !! */
		hist_item = -1;
	else
		hist_item = atoi(argv0);

	char *h = get_hist_entry(hist_item);

	if (!h)
		return;

	struct cmd *hist_cmd = parseline(h);
	puts(h);
	runcmd(hist_cmd);
	free_cmd(hist_cmd);
}

int main()
{
	hist_init();
	char line[4096];

	while(getcmd(line, sizeof(line))) {
		struct cmd *cmd = parseline(line);

		if (!cmd)
			continue;

		runcmd(cmd);
		free_cmd(cmd);
	}
	hist_clean();
	printf("\n");
}
