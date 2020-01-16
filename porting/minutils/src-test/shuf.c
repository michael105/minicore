#define _POSIX_C_SOURCE		200809L

#include <unistd.h>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define UNUSED(x)				((void)(x))
#define ARRAY_SIZE(x)				(sizeof(x) / sizeof((x)[0]))
#define SWAP(x,y,t)				\
	do {					\
		t __temp_var = (x);		\
		(x) = (y);			\
		(y) = __temp_var;		\
	} while (0)
#define STRIP_NEWLINE(s)			\
	do {					\
		char *ptr;			\
		if ((ptr = strchr(s, '\n')))	\
			*ptr = '\0';		\
	} while (0)

static struct {
	const char *argv0;
	const char *input;
	FILE *input_fh;
	unsigned int head;
	bool args : 1;
	bool zero : 1;
} opt;

static struct {
	const char **array;
	size_t len;
	size_t cap;
} lines;

static void resize_lines(size_t new_cap)
{
	void *ptr;

	ptr = realloc(lines.array, new_cap * sizeof(const char *));
	if (!ptr) {
		fprintf(stderr, "%s: %s\n",
			opt.argv0, strerror(errno));
		exit(1);
	}

	lines.array = ptr;
	lines.cap = new_cap;
}

static void append_line(const char *line)
{
	if (lines.len >= lines.cap)
		resize_lines(lines.cap * 2 + 1);

	lines.array[lines.len++] = line;
}

static void shuffle_lines(void)
{
	size_t i, j;

	for (i = lines.len - 1; i >= 1; i--) {
		j = rand() % (i + 1);
		if (i != j)
			SWAP(lines.array[i],
			     lines.array[j],
			     const char *);
	}
}

static int print_null(const char *str)
{
	return fwrite(str, strlen(str) + 1, 1, stdout) != 1;
}

static int print_newline(const char *str)
{
	return puts(str) == EOF;
}

static void print_lines(void)
{
	int (*print_func)(const char *);
	size_t i;

	print_func = (opt.zero)
		? &print_null
		: &print_newline;

	for (i = 0; i < lines.len; i++) {
		if (print_func(lines.array[i])) {
			fprintf(stderr, "%s: unable to write\n",
				opt.argv0);
			exit(1);
		}
	}
}

static void read_lines(void)
{
	char *line;
	size_t len;
	unsigned int i;

	for (i = 0; !opt.head || i < opt.head; i++) {
		line = NULL;
		len = 0;
		if (getline(&line, &len, opt.input_fh) < 0) {
			free(line);
			if (errno) {
				fprintf(stderr, "%s: %s: %s\n",
					opt.argv0,
					opt.input,
					strerror(errno));
				exit(1);
			}
			break;
		}
		STRIP_NEWLINE(line);
		append_line(line);
	}
}

static int parse_uint(unsigned int *res, const char *str)
{
	long val;
	char *ptr;

	val = strtol(str, &ptr, 10);
	if (!*str || *ptr || errno || val <= 0)
		return -1;

	*res = val;
	return 0;
}

/* Usage: shuf [-z] [FILE] */
/* Usage: shuf -e [-z] [ARG]... */
int main(int argc, char *argv[])
{
	int ch, i;

	opt.argv0 = argv[0];
	resize_lines(32);
	srand(time(NULL));

	/* Parse arguments */
	while ((ch = getopt(argc, argv, "ezn:")) != -1) {
		switch (ch) {
		case 'e':
			opt.args = 1;
			break;
		case 'z':
			opt.zero = 1;
			break;
		case 'n':
			if (parse_uint(&opt.head, optarg)) {
				fprintf(stderr, "%s: not a positive number: %s\n",
					argv[0], optarg);
				return 1;
			}
		case '?':
			return 1;
		default:
			abort();
		}
	}

	/* Read lines */
	if (opt.args) {
		for (i = optind; i < argc; i++)
			append_line(argv[i]);
	} else {
		switch (argc - optind) {
		case 0:
			opt.input = "<stdin>";
			opt.input_fh = stdin;
			break;
		case 1:
			opt.input = argv[optind];
			opt.input_fh = fopen(opt.input, "r");
			if (!opt.input) {
				fprintf(stderr, "%s: %s: %s\n",
					argv[0], opt.input, strerror(errno));
				return 1;
			}
			break;
		default:
			fprintf(stderr, "%s: at most one file may be listed\n",
				argv[0]);
			return 1;
		}
		read_lines();
	}

	/* Shuffle and print */
	shuffle_lines();
	print_lines();
	return 0;
}
