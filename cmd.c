
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"

#define PROMPT "dsh> "
#define WHITESPACE " \t\r\n\v"
#define SYMBOLS
#define DELIMS WHITESPACE SYMBOLS


int getcmd(char *buf, int sz)
{
	printf(PROMPT);
	fflush(stdout);
	if (fgets(buf, sz, stdin) == NULL)
		return 0;
	return 1;
}

struct cmd *parseline(char *line)
{
	struct cmd *cmd = malloc(sizeof(struct cmd));
	line = strdup(line);
	*cmd = (struct cmd){.base=line,.cl=strdup(line)};

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

	/* null terminate argv for execvp */
	cmd->argv = realloc(cmd->argv, sizeof(char*) * (cmd->argc+1));
	cmd->argv[cmd->argc] = NULL;

	return cmd;
}

void free_cmd(struct cmd *cmd)
{
	if (!cmd)
		return;
	free(cmd->cl);
	free(cmd->base);
	free(cmd->argv);
	free(cmd);
}
