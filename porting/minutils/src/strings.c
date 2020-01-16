#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct {
	const char *argv0;
	unsigned int min_len;
} opt;

static int isprint_lookahead(const char *buf, size_t index, size_t len)
{
	unsigned int i;

	for (i = 1; i < opt.min_len; i++) {
		if (index + i >= len)
			break;
		if (!isprint(buf[index + i]))
			return 0;
	}
	return 1;
}

static int strings(FILE *fh)
{
	char buf[4096];
	size_t i, len;
	int wip, ret;

	wip = 0;
	for (;;) {
		len = fread(buf, 1, sizeof(buf), fh);
		if (len < sizeof(buf)) {
			if (ferror(fh)) {
				ret = 1;
				break;
			}
		}
		for (i = 0; i < len; i++) {
			if (isprint(buf[i])) {
				if (!wip && isprint_lookahead(buf, i, len))
					wip = 1;
				if (wip)
					putchar(buf[i]);
			} else if (wip) {
				putchar('\n');
				wip = 0;
			}
		}
		if (feof(fh)) {
			ret = 0;
			break;
		}
	}
	if (wip)
		putchar('\n');
	return ret;
}

/* Usage: strings [-MIN-LENGTH] FILE... */
int main(int argc, const char *argv[])
{
	int i;

	opt.argv0 = argv[0];
	opt.min_len = 2;

	i = 1;
	if (i < argc && argv[i][0] == '-') {
		opt.min_len = atoi(argv[i] + 1);
		if (!opt.min_len) {
			fprintf(stderr, "%s: invalid number: %s\n",
				argv[0], argv[i]);
			return 1;
		}
		i++;
	}

	if (i == argc) {
		return strings(stdin);
	} else for (; i < argc; i++) {
		FILE *fh;
		int ret;

		fh = fopen(argv[i], "rb");
		if (!fh) {
			fprintf(stderr, "%s: unable to open '%s': %s\n",
				argv[0], argv[i], strerror(errno));
			return 1;
		}
		ret = strings(fh);
		fclose(fh);
		if (ret)
			return 1;
	}
	return 0;
}
