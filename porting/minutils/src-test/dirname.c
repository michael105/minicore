#include <libgen.h>

#include <stdio.h>

/* Usage: dirname PATH... */
int main(int argc, char *argv[])
{
	int i;

	if (argc < 2) {
		fprintf(stderr, "%s: missing operand\n", argv[0]);
		return 1;
	}

	for (i = 1; i < argc; i++)
		puts(dirname(argv[i]));
	return 0;
}
