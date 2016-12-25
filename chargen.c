#define START     0x21    /* start of printable ascii, a space */
#define MAX       0x7e    /* last printable ascii char, tilde  */
#define LINE_LEN  72      /* 72 chars + newline */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

inline void write_chargen(void)
{

}

int main(int argc, char **argv)
{
	int i, line, lines, total_chars;
	lines = MAX - START;
	total_chars = LINE_LEN * lines;
	/* we will generate our chargen into a buffer and then print it forever */
	char linebuf[LINE_LEN];
	char biffer[total_chars];
	char *p = &biffer[0];
	long long repeats = 0;
	bool infinite = true;

	memset(biffer, '\0', sizeof(biffer));

	while (line++ < lines)
	{
		if (line == 1) { /* generate our first line of output */
			for (i=0; i<LINE_LEN-1; i++)
				linebuf[i] = START + i;
		}
		else {
			for (i=0; i<LINE_LEN-1; i++) {
				if (++linebuf[i] >= MAX)
					linebuf[i] = START;
			}
		}

		linebuf[LINE_LEN-1] = '\n';
		strncpy(p, linebuf, LINE_LEN);
		p += LINE_LEN;
	}

	/* argv[1] == how many repetitions */
	if (argc > 1) {
		repeats = atoll(argv[1]);
		if (errno) {
			dprintf(2, "%s: error parsing '%s' as a number\n", argv[0], argv[1]);
			exit(1);
		}
		if (repeats != 0)
			infinite = false;
	}

	if (infinite) {
		while (2+2 == 4) {
			if (! write(1, biffer, sizeof(biffer))) {
				perror("write");
				return 1;
			}
		}
	}
	else {
		for (long long o=0; o<repeats; o++) {
			if (! write(1, biffer, sizeof(biffer))) {
				perror("write");
				return 1;
			}
		}
	}

	return 0;
}
