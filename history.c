
#define HISTSIZE 50

extern char **history;
extern size_t hist_entry;

static int hist_width;
static int hist_full;

void add_hist_entry(char *line)
{
	if (*line == ' ')
		return;

	history[hist_entry] = line;
	
	if (++hist_entry == HISTSIZE) {
		hist_entry = 0;
		hist_full = 1;
	}
}

int list_hist(int n)
{
	int i;

	if (n > HISTSIZE) {
		return 1;
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
	} else {
		return 1;
	}
}

int ndigits(int n)
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
}
