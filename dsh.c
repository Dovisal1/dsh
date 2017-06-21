/* \file dsh.c
 *
 * A simple shell
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
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

int main()
{
	char cmd[4096];

	while(getcmd(cmd, sizeof(cmd))) {
		printf("%s", cmd);
	}
	printf("\n");
}
