#define _XOPEN_SOURCE	500

#include <unistd.h>

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LINE_CAPACITY	32
#define DEFAULT_CHAR_CAPACITY	64

#define MIN(x, y)		(((x) < (y)) ? (x) : (y))
#define MAX(x, y)		(((x) > (y)) ? (x) : (y))

static const char *argv0;

static struct {
	char **array;
	size_t length;
	size_t capacity;
} lines;

static struct {
	char *array;
	size_t length;
	size_t capacity;
} line;

static enum {
	MODE_BYTES,
	MODE_LINES
} mode;
static size_t count;
static bool quiet = false;

static void append_to_line_list(void)
{
	char *str;

	if (lines.length >= lines.capacity) {
		void *ptr;
		size_t new_capacity;

		new_capacity = lines.capacity * 2;
		ptr = realloc(lines.array, sizeof(char *) * new_capacity);
		if (!ptr) {
			fprintf(stderr, "%s: %s\n",
				argv0, strerror(errno));
			exit(-1);
		}
		lines.array = ptr;
		lines.capacity = new_capacity;
	}

	str = malloc(line.length + 1);
	if (!str) {
		fprintf(stderr, "%s: %s\n",
			argv0, strerror(errno));
		exit(1);
	}

	memcpy(str, line.array, line.length);
	str[line.length] = '\0';
	lines.array[lines.length++] = str;
}

static void append_to_char_list(char ch)
{
	if (line.length >= line.capacity) {
		void *ptr;
		size_t new_capacity;

		new_capacity = line.capacity * 2;
		ptr = realloc(line.array, sizeof(char) * new_capacity);
		if (!ptr) {
			fprintf(stderr, "%s: %s\n",
				argv0, strerror(errno));
			exit(-1);
		}
		line.array = ptr;
		line.capacity = new_capacity;
	}
	line.array[line.length++] = ch;
}

static void clear_lines(void)
{
	size_t i;

	for (i = 0; i < lines.length; i++) {
		free(lines.array[i]);
	}
	lines.length = 0;

	free(line.array);
	line.length = 0;
}

static void parse_arg(const char *arg)
{
	long num;
	char *ptr;

	num = strtol(optarg, &ptr, 0);
	if (*ptr) {
		fprintf(stderr, "%s: '%s' not a valid number for '%s'\n",
			argv0, optarg, arg);
		exit(1);
	}
	count = num;
}

static void print_file(const char *filename)
{
	size_t i;

	if (filename && !quiet)
		printf("[%s]\n", filename);

#ifndef BOTTOM_SLICE
	/* head */
	switch (mode) {
		case MODE_BYTES:
			count = MIN(line.length, count);
			if (fwrite(line.array, 1, count, stdout) < count) {
				exit(1);
			}
			break;
		case MODE_LINES:
			count = MIN(lines.length, count);
			for (i = 0; i < count; i++) {
				puts(lines.array[i]);
			}
			break;
		default:
			abort();
	}
#else
	/* tail */
	switch (mode) {
		case MODE_BYTES:
			count = MIN(line.length, count);
			if (fwrite(&line.array[line.length - count - 1], 1, count, stdout) < count) {
				exit(1);
			}
			break;
		case MODE_LINES:
			count = MIN(lines.length, count);
			for (i = lines.length - count - 1; i < lines.length; i++) {
				puts(lines.array[i]);
			}
			break;
		default:
			abort();
	}
#endif /* !BOTTOM_SLICE */

	if (filename && !quiet)
		putchar('\n');
}

/* Usage: head [-n LINES] [-c bytes] [-q] [-v] [FILE...] */
int main(int argc, char *argv[])
{
	int i, ch;

	mode = MODE_LINES;
	count = 10;

	while ((ch = getopt(argc, argv, "n:c:qv")) != -1) {
		switch (ch) {
			case 'n':
				parse_arg("-n");
				break;
			case 'c':
				parse_arg("-c");
				mode = MODE_BYTES;
				break;
			case 'q':
				quiet = true;
				break;
			case 'v':
				quiet = false;
				break;
			case '?':
				return 1;
			default:
				abort();
		}
	}

	lines.array = malloc(sizeof(char *) * DEFAULT_LINE_CAPACITY);
	lines.length = 0;
	lines.capacity = DEFAULT_LINE_CAPACITY;

	if (!lines.array) {
		fprintf(stderr, "%s: %s\n",
				argv[0], strerror(errno));
		return 1;
	}

	line.array = malloc(sizeof(char) * DEFAULT_CHAR_CAPACITY);
	line.length = 0;
	line.capacity = DEFAULT_CHAR_CAPACITY;

	if (!line.array) {
		fprintf(stderr, "%s: %s\n",
				argv[0], strerror(errno));
		return 1;
	}

	if (optind == argc) {
		while ((ch = fgetc(stdin)) != EOF) {
			if (ch == '\n' && mode == MODE_LINES) {
				append_to_line_list();
				line.length = 0;
#ifndef BOTTOM_SLICE
				/* head */
				if (lines.length >= count)
					break;
#endif /* !BOTTOM_SLICE */
			} else {
				append_to_char_list(ch);
#ifndef BOTTOM_SLICE
				/* head */
				if (mode == MODE_BYTES && line.length >= count)
					break;
#endif /* !BOTTOM_SLICE */
			}
		}
		if (mode == MODE_LINES) {
			append_to_line_list();
		}
		print_file(NULL);
	} else {
		for (i = optind; i < argc; i++) {
			FILE *fh;

			if (!strcmp(argv[i], "-"))
				fh = stdin;
			else
				fh = fopen(argv[i], "r");

			if (!fh)
				return 1;
			while ((ch = fgetc(fh)) != EOF) {
				if (ch == '\n' && mode == MODE_LINES) {
					append_to_line_list();
					line.length = 0;
				} else {
					append_to_char_list(ch);
				}
			}
			if (fclose(fh)) {
				fprintf(stderr, "%s: %s\n",
						argv[0], strerror(errno));
				return 1;
			}
			if (mode == MODE_LINES) {
				append_to_line_list();
			}
			print_file(argv[i]);
			clear_lines();
		}
	}

	return 0;
}
