#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct {
	const char *argv0;
	bool quiet : 1;
} opt;

static FILE *open_file(const char *path)
{
	FILE *fh;

	if (!strcmp(path, "-"))
		return stdin;
	fh = fopen(path, "r");
	if (!fh) {
		fprintf(stderr, "%s: %s: %s\n",
			opt.argv0, path, strerror(errno));
		exit(2);
	}
	return fh;
}

static long parse_int(const char *str)
{
	char *ptr;
	long val;

	val = strtol(str, &ptr, 0);
	if (*ptr || val < 0) {
		fprintf(stderr, "%s: invalid offset: %s\n",
			opt.argv0, str);
		exit(2);
	}
	return val;
}

static int do_compare(FILE *fh1, FILE *fh2)
{
	int ch1, ch2;
	size_t byte;

	if (fh1 == fh2)
		return 0;

	byte = 0;
	do {
		ch1 = getc(fh1);
		ch2 = getc(fh2);
		if (ch1 != ch2) {
			if (!opt.quiet) {
				printf("files differ at byte %lu\n",
					(unsigned long)byte);
			}
			return 1;
		}
		byte++;
	} while (ch1 != EOF);

	if (fclose(fh1)) {
		fprintf(stderr, "%s: %s\n",
			opt.argv0, strerror(errno));
		return 2;
	}
	if (fclose(fh2)) {
		fprintf(stderr, "%s: %s\n",
			opt.argv0, strerror(errno));
		return 2;
	}
	return 0;
}

/* Usage: cmp [-q] FILE1 [FILE2 [SKIP1 [SKIP2]]] */
int main(int argc, const char *argv[])
{
	FILE *fh1, *fh2;
	long skip1, skip2;
	int i;

	opt.argv0 = argv[0];
	fh2 = stdin;
	skip1 = 0;
	skip2 = 0;
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			break;
		} else if (!strcmp(argv[i], "-q")) {
			opt.quiet = 1;
		} else {
			fprintf(stderr, "%s: invalid argument: %s\n",
				argv[0], argv[i]);
			return 2;
		}
	}

	switch (argc - i) {
	case 4:
		skip2 = parse_int(argv[i + 3]);
		/* FALLTHROUGH */
	case 3:
		skip1 = parse_int(argv[i + 2]);
		/* FALLTHROGH */
	case 2:
		fh2 = open_file(argv[i + 1]);
		/* FALLTHROUGH */
	case 1:
		fh1 = open_file(argv[i]);
		break;
	default:
		fprintf(stderr, "usage: %s [-q] FILE1 [FILE2 [SKIP1 [SKIP2]]]\n",
			argv[0]);
		return 2;
	}

	if (skip1 && fseek(fh1, skip1, SEEK_SET) < 0) {
		fprintf(stderr, "%s: unable to seek: %s\n",
			argv[0], strerror(errno));
		return 2;
	}
	if (skip2 && fseek(fh2, skip2, SEEK_SET) < 0) {
		fprintf(stderr, "%s: unable to seek: %s\n",
			argv[0], strerror(errno));
		return 2;
	}
	return do_compare(fh1, fh2);
}
