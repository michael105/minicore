#define _DEFAULT_SOURCE

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct {
	const char *argv0;
	bool dir : 1;
} opt;

static int get_suffix(const char *str)
{
	size_t len;
	int i;

	len = strlen(str);
	for (i = len - 1; i >= 0; i--) {
		if (str[i] == 'X')
			return len - i - 1;
	}
	exit(-1);
}

static int make_temp_dir(char *template)
{
	if (!mkdtemp(template)) {
		if (errno == EINVAL)
			fprintf(stderr, "%s: template does not end in exactly 'XXXXX': %s\n",
				opt.argv0, template);
		else
			fprintf(stderr, "%s: %s\n",
				opt.argv0, strerror(errno));
		return -1;
	}
	return 0;
}

static int make_temp_file(char *template)
{
	int fd;

	if (!strstr(template, "XXXXX")) {
		fprintf(stderr, "%s: too few X's in template: %s\n",
			opt.argv0, template);
		return -1;
	}
	fd = mkstemps(template, get_suffix(template));
	if (fd < 0) {
		fprintf(stderr, "%s: %s\n",
			opt.argv0, strerror(errno));
		return -1;
	}
	if (close(fd)) {
		fprintf(stderr, "%s: %s: %s\n",
			opt.argv0, template, strerror(errno));
		return -1;
	}
	return 0;
}

/* Usage: mktemp [-d] TEMPLATE */
int main(int argc, char *argv[])
{
	char *template;
	int i;

	opt.argv0 = argv[0];
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			break;
		} else if (!strcmp("-d", argv[i])) {
			opt.dir = 1;
		} else {
			fprintf(stderr, "%s: invalid argument: %s\n",
				argv[0], argv[i]);
		}
	}
	if (i == argc) {
		fprintf(stderr, "%s: missing operand\n", argv[0]);
		return 1;
	} else if (i < argc - 1) {
		fprintf(stderr, "%s: extra operands\n", argv[0]);
		return 1;
	}
	template = argv[i];
	if (opt.dir) {
		if (make_temp_dir(template))
			return 1;
	} else {
		if (make_temp_file(template))
			return 1;
	}
	puts(template);
	return 0;
}
