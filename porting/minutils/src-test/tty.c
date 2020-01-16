#include <unistd.h>

#include <stdio.h>
#include <string.h>

/* Usage: tty [-s] */
int main(int argc, const char *argv[])
{
	const char *tty;
	int verbose, i;

	verbose = 1;
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-s")) {
			verbose = 0;
		} else {
			fprintf(stderr, "%s: invalid argument: %s\n",
				argv[0], argv[i]);
			return 2;
		}
	}
	tty = ttyname(STDIN_FILENO);
	if (verbose) {
		if (tty)
			puts(tty);
		else
			puts("not a tty");
	}
	return tty == NULL;
}
