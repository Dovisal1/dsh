/* \file dsh.c
 *
 * A simple shell
 */

#include <stdio.h>
#include <unistd.h>

#define PROMPT "dsh> "

static int getcmd(char *buf, int sz)
{
	printf(PROMPT);
	fflush(stdout);
	if (fgets(buf, sz, stdin) == NULL)
		return 0;
	return 1;
}


int main()
{
	char cmd[4096];

	while(getcmd(cmd, sizeof(cmd))) {
		printf("%s", cmd);
	}
	printf("\n");
}
