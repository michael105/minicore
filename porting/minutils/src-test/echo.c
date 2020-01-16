#include <stdio.h>

/* Usage: echo [TEXT...] */
int main(int argc, char *argv[])
{
	int i;

	if (argc == 1) {
		putchar('\n');
		return 0;
	}
	for (i = 1; i < argc; i++) {
		fputs(argv[i], stdout);
		if (i < argc - 1)
			putchar(' ');
	}
	putchar('\n');
	return 0;
}
