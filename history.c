
#include <stdio.h>
#include <string.h>

#define HISTSIZE 50

char **history;
size_t hist_entry;
int hist_width;
int hist_full;

void add_hist_entry(char *line)
{
	if (*line == ' ')
		return;

	if (history[hist_entry])
		free(history[hist_entry]);

	history[hist_entry] = strdup(line);
	
	if (++hist_entry == HISTSIZE) {
		hist_entry = 0;
		hist_full = 1;
	}
}

char *get_entry(int n)
{
	if (!n) {
		return history[hist_entry];
	} else if (n > 0) {
		int ind = hist_entry - n;
		if (ind < 0)
			ind += HISTSIZE;
		return history[ind];
	} else {
		return NULL;
	}
}

void list_hist(int n)
{
	int i;

	if (n > HISTSIZE || n < 0) {
		fprintf(stderr, "dsh: history: invalid arg\n");
	} else if (n > 0) {
		i = hist_entry - n;

		if (i < 0)
			i += HISTSIZE;

		for (; i != hist_entry; i = (i+1) % HISTSIZE)
			printf(" %*d  %s\n", hist_width, i, history[i]);
	} else if (!n) {
		i = (hist_entry+1) % HISTSIZE;

		for(; i != hist_entry; i = (i+1) % HISTSIZE)
			printf(" %*d  %s\n", hist_width, i, history[i]);
	}
}

static int ndigits(int n)
{
	int r;
	for(r = 0; n; r++)
		n /= 10;
	return r;
}

void hist_init()
{
	hist_width = ndigits(HISTSIZE);
	hist_full = 0;
	hist_entry = 0;
	history = malloc(HISTSIZE * sizeof(char*));

	for (int i = 0; i < HISTSIZE; i++)
		history[i] = NULL;
}
