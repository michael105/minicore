#define _XOPEN_SOURCE	500

#include <unistd.h>

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct {
	unsigned long bytes;
	unsigned long lines;
	unsigned long words;
} counts;

static struct {
	bool bytes : 1;
	bool lines : 1;
	bool words : 1;
} opt;

static int ret;

static void count(FILE *fh)
{
	char buf[4096];
	size_t i, bytes;
	bool inspace;

	inspace = 1;
	memset(&counts, 0, sizeof(counts));
	do {
		bytes = fread(buf, 1, sizeof(buf), fh);
		counts.bytes += bytes;
		for (i = 0; i < bytes; i++) {
			if (buf[i] == '\n')
				counts.lines++;
			if (isspace(buf[i])) {
				if (!inspace) {
					inspace = 1;
					counts.words++;
				}
			} else {
				inspace = 0;
			}
		}

		if (feof(fh) || ferror(fh))
			break;
	} while (bytes);
}

static void print_counts(const char *fn)
{
	if (opt.lines)
		printf("%lu ", counts.lines);
	if (opt.words)
		printf("%lu ", counts.words);
	if (opt.bytes)
		printf("%lu ", counts.bytes);
	puts(fn);
}

/* Usage: wc [-c] [-l] [-w] [FILE...] */
int main(int argc, char *argv[])
{
	int i, ch;

	while ((ch = getopt(argc, argv, "clw")) != -1) {
		switch (ch) {
		case 'c':
			opt.bytes = 1;
			break;
		case 'l':
			opt.lines = 1;
			break;
		case 'w':
			opt.words = 1;
			break;
		case '?':
			return 1;
		default:
			abort();
		}
	}
	if (optind == 1) {
		opt.bytes = 1;
		opt.lines = 1;
		opt.words = 1;
	}

	if (optind == argc) {
		count(stdin);
		print_counts("");
	} else for (i = optind; i < argc; i++) {
		FILE *fh;
		bool opened;

		if (strcmp(argv[i], "-")) {
			fh = fopen(argv[i], "r");
			if (!fh) {
				fprintf(stderr, "%s: %s: %s\n",
					argv[0], argv[1], strerror(errno));
				continue;
			}
			opened = 1;
		} else {
			fh = stdin;
			opened = 0;
		}

		count(fh);
		print_counts(argv[i]);
		if (opened)
			fclose(fh);
	}

	return ret;
}
