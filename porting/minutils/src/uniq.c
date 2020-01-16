#define _POSIX_C_SOURCE		200809L

#include <unistd.h>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ARRAY_SIZE(x)		(sizeof(x) / sizeof((x)[0]))
#define UNUSED(x)		((void)(x))

static struct {
	const char *argv0;
	const char *input;
	const char *output;
	FILE *input_fh;
	FILE *output_fh;
	bool uniq  : 1;
	bool count : 1;
} opt;

static struct {
	struct bucket {
		char *str;
		unsigned long count;
		struct bucket *next;
	} buckets[256];
} lines;

typedef int (*line_iter_cbf)(void *, char *, unsigned long);

static unsigned long djb2_hash(const char *str)
{
	unsigned long hash = 5381;

	while (*str++)
		hash = ((hash << 5) + hash) + *str;
	return hash;
}

static struct bucket *get_bucket(char *line)
{
	unsigned long hash;
	size_t index;

	hash = djb2_hash(line);
	index = hash % ARRAY_SIZE(lines.buckets);
	return &lines.buckets[index];
}

static int line_add(char *line)
{
	struct bucket *bucket;

	bucket = get_bucket(line);
	do {
		if (!bucket->str) {
			bucket->str = line;
			bucket->count = 1;
			return 0;
		}
		if (!strcmp(bucket->str, line)) {
			free(line);
			bucket->count++;
			return 0;
		}
		if (!bucket->next) {
			bucket->next = calloc(1, sizeof(struct bucket));
			if (!bucket->next)
				return -1;
		}
		bucket = bucket->next;
	} while (bucket->next);
	abort();
}

static int line_iterate(line_iter_cbf cbf, void *arg)
{
	const struct bucket *bucket;
	size_t i;

	for (i = 0; i < ARRAY_SIZE(lines.buckets); i++) {
		bucket = &lines.buckets[i];
		if (!bucket->str)
			continue;
		if (cbf(arg, bucket->str, bucket->count))
			return -1;
		while (bucket->next) {
			bucket = bucket->next;
			if (!bucket->str)
				break;
			if (cbf(arg, bucket->str, bucket->count))
				return -1;
		}
	}
	return 0;
}

static int print_count(void *arg, char *line, unsigned long count)
{
	int errsave;

	UNUSED(arg);

	fprintf(opt.output_fh, "%4lu %s", count, line);
	errsave = errno;
	free(line);
	errno = errsave;
	return !!errsave;
}

static int print_only(void *arg, char *line, unsigned long count)
{
	int errsave;

	UNUSED(arg);
	UNUSED(count);

	fputs(line, opt.output_fh);
	errsave = errno;
	free(line);
	errno = errsave;
	return !!errsave;
}

static int slurp_and_print(void)
{
	line_iter_cbf cbf;
	char *line;
	size_t len;

	do {
		line = NULL;
		len = 0;
		if (getline(&line, &len, opt.input_fh) < 0) {
			free(line);
			if (errno) {
				fprintf(stderr, "%s: %s: %s\n",
					opt.argv0, opt.input, strerror(errno));
				return 1;
			}
			break;
		}
		if (line_add(line)) {
			free(line);
			fprintf(stderr, "%s: %s\n",
				opt.argv0, strerror(errno));
			return 1;
		}
	} while (line);

	cbf = (opt.count) ? &print_count : &print_only;
	if (line_iterate(cbf, NULL)) {
		fprintf(stderr, "%s: %s: %s\n",
			opt.argv0, opt.output, strerror(errno));
		return 1;
	}
	return 0;
}

static int print_non_unique(void)
{
	char *line, *last;
	size_t len;

	line = NULL;
	len = 0;
	if (getline(&line, &len, opt.input_fh) < 0) {
		if (errno) {
			fprintf(stderr, "%s: %s: %s\n",
				opt.argv0, opt.input, strerror(errno));
			free(line);
			return 1;
		}
		if (fputs(line, opt.output_fh) == EOF) {
			fprintf(stderr, "%s: %s: %s\n",
				opt.argv0, opt.output, strerror(errno));
			free(line);
			return 1;
		}
		free(line);
		return 0;
	}

	if (fputs(line, opt.output_fh) == EOF) {
		fprintf(stderr, "%s: %s: %s\n",
			opt.argv0, opt.output, strerror(errno));
		return 1;
	}
	last = line;

	do {
		line = NULL;
		len = 0;
		if (getline(&line, &len, opt.input_fh) < 0) {
			free(line);
			if (errno) {
				fprintf(stderr, "%s: %s: %s\n",
					opt.argv0, opt.input, strerror(errno));
				return 1;
			}
			break;
		}

		if (strcmp(line, last)) {
			if (fputs(line, opt.output_fh) == EOF) {
				fprintf(stderr, "%s: %s: %s\n",
					opt.argv0, opt.output, strerror(errno));
				return 1;
			}
		}
		free(last);
		last = line;
	} while (line);
	free(last);
	return 0;
}

/* Usage: uniq [-u] [-c] [INPUT [OUTPUT]] */
int main(int argc, char *argv[])
{
	int ch;

	opt.argv0 = argv[0];

	/* Parse arguments */
	while ((ch = getopt(argc, argv, "uc")) != -1) {
		switch (ch) {
		case 'u':
			opt.uniq = 1;
			break;
		case 'c':
			opt.count = 1;
			break;
		case '?':
			return 1;
		default:
			abort();
		}
	}
	switch (argc - optind) {
	case 2:
		opt.output = argv[2];
		/* FALLTHROUGH */
	case 1:
		opt.input = argv[1];
		/* FALLTHROUGH */
	case 0:
		break;
	default:
		fprintf(stderr, "usage: %s [-u] [INPUT [OUTPUT]]\n", argv[0]);
		return 1;
	}

	/* Open files */
	if (opt.input) {
		opt.input_fh = fopen(opt.input, "r");
		if (!opt.input_fh) {
			fprintf(stderr, "%s: %s: %s\n",
				argv[0], opt.input, strerror(errno));
			return 1;
		}
	} else {
		opt.input = "<stdin>";
		opt.input_fh = stdin;
	}
	if (opt.output) {
		opt.output_fh = fopen(opt.output, "w");
		if (!opt.output_fh) {
			fprintf(stderr, "%s: %s: %s\n",
				argv[0], opt.output, strerror(errno));
			return 1;
		}
	} else {
		opt.output = "<stdout>";
		opt.output_fh = stdout;
	}

	/* Read and print lines */
	if (opt.uniq)
		return slurp_and_print();
	else
		return print_non_unique();
}
