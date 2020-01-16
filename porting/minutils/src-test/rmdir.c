#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

/* Usage: rmdir [PATH...] */
int main(int argc, const char *argv[])
{
	int i;

	if (argc == 1) {
		fprintf(stderr, "%s: missing operand\n",
			argv[0]);
		return 1;
	}
	for (i = 1; i < argc; i++) {
		if (rmdir(argv[i])) {
			fprintf(stderr, "%s: %s: %s\n",
				argv[0], argv[i], strerror(errno));
			return 1;
		}
	}
	return 0;
}
