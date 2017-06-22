
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "history.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

const char *builtins[] = {
	"echo",
	"cd",
	"history",
};

const size_t n_builtins = ARRAY_SIZE(builtins);

int is_builtin(struct cmd *cmd)
{
	for (size_t i = 0; i < n_builtins; i++)
		if (!strcmp(cmd->argv[0], builtins[i]))
			return 1;
	return 0;
}

size_t builtin_num(struct cmd * cmd)
{
	for (int i = 0; i < n_builtins; i++)
		if (!strcmp(cmd->argv[0], builtins[i]))
			return i;
	return n_builtins;	/* error */
}

void run_echo(struct cmd *cmd)
{
	for (size_t i = 1; i < cmd->argc; i++)
		printf("%s%c", cmd->argv[i], i + 1 < cmd->argc ? ' ' : '\n');
}

void run_cd(struct cmd *cmd)
{
	if (cmd->argc > 2) {
		fprintf(stderr, "dsh: cd: too many args\n");
	} else if (chdir(cmd->argv[1])) {
		fprintf(stderr, "dsh: cd: ");
		perror(cmd->argv[1]);
	}
}

void run_history(struct cmd *cmd)
{
	list_hist(cmd->argc == 1 ? 0 : atoi(cmd->argv[1]));
}
