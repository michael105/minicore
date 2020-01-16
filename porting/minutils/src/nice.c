#define _XOPEN_SOURCE	500

#include <sys/resource.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static struct {
	const char *argv0;
	int nice;
} opt;

static int parse_nice(const char *str)
{
	long val;
	char *ptr;

	val = strtol(str, &ptr, 10);
	if (!*str || *ptr || errno) {
		fprintf(stderr, "%s: invalid niceness value: %s\n",
			opt.argv0, str);
		return -1;
	}

	opt.nice = val;
	return 0;
}

/* Usage: nice [-n ADJUSTMENT] [COMMAND [ARG]...] */
int main(int argc, char *argv[])
{
	int ch;

	opt.argv0 = argv[0];
	opt.nice = 10;

	while ((ch = getopt(argc, argv, "n:")) != -1) {
		switch (ch) {
		case 'n':
			if (parse_nice(optarg))
				return 1;
			break;
		case '?':
			return 1;
		default:
			abort();
		}
	}
	if (argc < 2) {
		/* No arguments, get current niceness */
		printf("%d\n", getpriority(PRIO_PROCESS, 0));
		return 0;
	}

	/* Nice self, and exec to new process */
	errno = 0;
	nice(opt.nice);
	if (errno) {
		fprintf(stderr, "%s: unable to increase niceness: %s\n",
			argv[0], strerror(errno));
		return 1;
	}
	execvp(argv[optind], &argv[optind]);
	fprintf(stderr, "%s: %s: unable to exec: %s\n",
		argv[0], argv[optind], strerror(errno));
	return 1;
}
