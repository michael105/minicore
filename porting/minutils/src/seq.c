#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct {
	const char *argv0;
	const char *sep;

	double first;
	double last;
	double increment;
} opt;

#define ISNAN(x)	((x) != (x))

static double parse_float(const char *str)
{
	char *ptr;
	double val;

	val = strtod(str, &ptr);
	if (*ptr) {
		fprintf(stderr, "%s: invalid float-point argument: %s\n",
			opt.argv0, str);
		exit(1);
	}
	return val;
}

/* Usage: seq [-s=SEPARATOR] [FIRST] [INCREMENT] LAST */
int main(int argc, const char *argv[])
{
	double n;
	int i;

	opt.argv0 = argv[0];
	opt.sep = "\n";
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			break;
		} else if (!strncmp(argv[i], "-s=", 3)) {
			opt.sep = argv[i] + 3;
		} else {
			char *ptr;

			strtod(argv[i], &ptr);
			if (!*ptr) {
				break;
			}
			fprintf(stderr, "%s: invalid argument: %s\n",
				argv[0], argv[i]);
			return 1;
		}
	}
	switch (argc - i) {
	case 1:
		opt.first = 1;
		opt.last = parse_float(argv[i]);
		opt.increment = 1;
		break;
	case 2:
		opt.first = parse_float(argv[i]);
		opt.last = parse_float(argv[i + 1]);
		opt.increment = 1;
		break;
	case 3:
		opt.first = parse_float(argv[i]);
		opt.last = parse_float(argv[i + 2]);
		opt.increment = parse_float(argv[i + 1]);
		break;
	case 0:
		fprintf(stderr, "%s: missing operand\n",
			opt.argv0);
		return 1;
	default:
		fprintf(stderr, "%s: extra operand: %s\n",
			opt.argv0, argv[i + 3]);
		return 1;
	}

	if (opt.increment == 0) {
		fprintf(stderr, "%s: zero increment\n", opt.argv0);
		return 1;
	}
	if (ISNAN(opt.first) || ISNAN(opt.last) || ISNAN(opt.increment)) {
		fprintf(stderr, "%s: nan values are disallowed\n", opt.argv0);
		return 1;
	}
	if ((opt.increment > 0) != (opt.first < opt.last)) {
		/* Can't reach target */
		return 0;
	}
	if (opt.increment > 0) {
		for (n = opt.first; n <= opt.last; n += opt.increment)
			printf("%g%s", n, opt.sep);
	} else {
		for (n = opt.first; n >= opt.last; n += opt.increment)
			printf("%g%s", n, opt.sep);
	}
	return 0;
}
