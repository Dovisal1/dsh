
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTSIZE 50

char **history;
size_t hist_entry;
int hist_width;

char *chomp(char *s)
{
	int last = strlen(s)-1;
	if(s[last] == '\n')
		s[last] = '\0';
	return s;
}

void add_hist_entry(char *line)
{
	if (*line == ' ' || *line == '\n')
		return;

	if (history[hist_entry])
		free(history[hist_entry]);

	history[hist_entry] = chomp(strdup(line));
	
	if (++hist_entry == HISTSIZE)
		hist_entry = 0;
}

char *get_hist_entry(int n)
{
	int ind;

	if (n > HISTSIZE || n < -HISTSIZE)
		return NULL;

	if (n >= 0)
		ind = n;
	else
		ind = hist_entry + n;

	if (hist_entry < 0)
		hist_entry += HISTSIZE;

	return history[ind];
}

void list_hist(int n)
{
	int i, order = 0;

	if (n > HISTSIZE || n < 0) {
		fprintf(stderr, "dsh: history: invalid arg\n");
		return;
	} else if (n > 0) {
		i = hist_entry - n;
		if (i < 0)
			i += HISTSIZE;
	} else  {
		i = (hist_entry+1) % HISTSIZE;
	}

	for(; i != hist_entry; i = (i+1) % HISTSIZE) {
		if(!history[i])
			continue;

		printf(" %*d  %s\n", hist_width, order, history[i]);
		order++; /* don't advance if skipping */
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
	hist_entry = 0;
	history = malloc(HISTSIZE * sizeof(char*));

	for (size_t i = 0; i < HISTSIZE; i++)
		history[i] = NULL;
}

void hist_clean()
{
	free(history);
}
