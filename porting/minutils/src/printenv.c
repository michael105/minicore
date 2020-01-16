#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern const char **environ;

static struct {
	const char *argv0;
	bool null : 1;
} opt;

static int putnull(const char *str)
{
	size_t len;

	len = strlen(str);
	if (fwrite(str, len + 1, 1, stdout) != 1)
		return EOF;
	return 0;
}

static void print_all(void)
{
	int (*print_func)(const char *);

	print_func = opt.null ? putnull : puts;
	while (*environ) {
		if (print_func(*environ++) == EOF) {
			fprintf(stderr, "%s: error while writing\n",
				opt.argv0);
			exit(1);
		}
	}
}

static char *format_var(const char *key, const char *val)
{
	size_t keylen, vallen;
	char *buf;

	keylen = strlen(key);
	vallen = strlen(val);
	buf = malloc(keylen + vallen + 2);
	sprintf(buf, "%s=%s", key, val);
	return buf;
}

static void print_vars(int len, const char *names[])
{
	int (*print_func)(const char *);
	int i, ret;

	ret = 0;
	print_func = opt.null ? putnull : puts;
	for (i = 0; i < len; i++) {
		const char *value;
		char *buf;

		value = getenv(names[i]);
		if (!value) {
			ret = 1;
			continue;
		}
		buf = format_var(names[i], value);
		if (!buf) {
			fprintf(stderr, "%s: unable to allocate: %s\n",
				opt.argv0, strerror(errno));
			exit(1);
		}
		if (print_func(buf) == EOF) {
			fprintf(stderr, "%s: error while writing\n",
				opt.argv0);
			exit(1);
		}
		free(buf);
	}
	exit(ret);
}

/* Usage: printenv [-0] [VARIABLE...] */
int main(int argc, const char *argv[])
{
	int i;

	opt.argv0 = argv[0];
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			break;
		} else if (!strcmp(argv[i], "-0")) {
			opt.null = 1;
		} else {
			fprintf(stderr, "%s: invalid argument: %s\n",
				argv[0], argv[i]);
			return 1;
		}
	}

	if (i == argc)
		print_all();
	else
		print_vars(argc - i, argv + i);
	return 0;
}
