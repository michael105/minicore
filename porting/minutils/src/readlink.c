#define _XOPEN_SOURCE	500

#include <limits.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct {
	const char *argv0;
	char end;
} opt;

static int do_dynamic_readlink(const char *path)
{
	char *buf;
	size_t len;
	ssize_t ret;

	len = PATH_MAX * 2;
	for (;;) {
		buf = malloc(len);
		if (!buf) {
			fprintf(stderr, "%s: unable to allocate: %s\n",
				opt.argv0, strerror(errno));
			return -1;
		}
		ret = readlink(path, buf, sizeof(buf));
		if (ret < 0) {
			fprintf(stderr, "%s: %s: %s\n",
				opt.argv0, path, strerror(errno));
			return -1;
		}
		if (ret == sizeof(buf)) {
			free(buf);
			len += PATH_MAX;
			continue;
		}
		buf[ret] = '\0';
		printf("%s%c", buf, opt.end);
		free(buf);
		return 0;
	}
	return 0;
}

static int do_readlink(const char *path)
{
	char buf[PATH_MAX];
	ssize_t ret;

	ret = readlink(path, buf, sizeof(buf));
	if (ret < 0) {
		fprintf(stderr, "%s: %s: %s\n",
			opt.argv0, path, strerror(errno));
		return -1;
	}
	if (ret == sizeof(buf))
		return do_dynamic_readlink(path);
	buf[ret] = '\0';
	printf("%s%c", buf, opt.end);
	return 0;
}

/* Usage: readlink [-n] [PATH...] */
int main(int argc, const char *argv[])
{
	int i, ret;

	opt.argv0 = argv[0];
	opt.end = '\n';
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			break;
		} else if (!strcmp(argv[i], "-n")) {
			opt.end = '\0';
		} else {
			fprintf(stderr, "%s: invalid argument: %s\n",
				argv[0], argv[i]);
			return 1;
		}

	}
	if (i == argc) {
		fprintf(stderr, "%s: missing operand\n", argv[0]);
		return 1;
	}

	ret = 0;
	for (; i < argc; i++) {
		if (do_readlink(argv[i]))
			ret = 1;
	}
	return ret;
}
