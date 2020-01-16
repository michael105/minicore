#define _POSIX_C_SOURCE		200809L

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <libgen.h>
#include <unistd.h>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct {
	const char *argv0;

	bool symbolic : 1;
} opt;

static int single_link(const char *source, const char *dest)
{
	if (opt.symbolic)
		return symlink(source, dest);
	else
		return link(source, dest);
}

static int multi_link(int count, const char *files[])
{
	struct stat statbuf;
	const char *dest;
	int i, destfd;

	dest = files[count - 1];
	destfd = open(dest, O_RDONLY);
	if (destfd < 0) {
		fprintf(stderr, "%s: %s: %s\n",
			opt.argv0, dest, strerror(errno));
		return 1;
	}
	if (fstat(destfd, &statbuf) || !S_ISDIR(statbuf.st_mode)) {
		fprintf(stderr, "%s: %s: not a directory\n",
			opt.argv0, dest);
		return 1;
	}
	if (opt.symbolic) {
		for (i = 0; i < count - 1; i++) {
			char *copy, *destfn;

			copy = strdup(files[i]);
			if (!copy) {
				fprintf(stderr, "%s: %s\n",
					opt.argv0, strerror(errno));
				return 1;
			}
			destfn = basename(copy);
			if (symlinkat(files[i], destfd, destfn)) {
				fprintf(stderr, "%s: %s: %s\n",
					opt.argv0, files[i], strerror(errno));
				free(copy);
				return 1;
			}
			free(copy);
		}
	} else {
		for (i = 0; i < count - 1; i++) {
			char *copy, *destfn;

			copy = strdup(files[i]);
			if (!copy) {
				fprintf(stderr, "%s: %s\n",
					opt.argv0, strerror(errno));
				return 1;
			}
			destfn = basename(copy);
			if (linkat(AT_FDCWD, files[i], destfd, destfn, 0)) {
				fprintf(stderr, "%s: %s: %s\n",
					opt.argv0, files[i], strerror(errno));
				free(copy);
				return 1;
			}
			free(copy);
		}
	}
	return 0;
}

/* Usage: ln [-s] SOURCE... DEST */
int main(int argc, const char *argv[])
{
	int i;

	opt.argv0 = argv[0];
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			break;
		} else if (!strcmp("-s", argv[i])) {
			opt.symbolic = 1;
		} else {
			fprintf(stderr, "%s: invalid argument: %s\n",
				argv[0], argv[i]);
			return 1;
		}
	}

	switch (argc - i) {
	case 0:
		fprintf(stderr, "%s: missing operand\n", argv[0]);
		return 1;
	case 1:
		fprintf(stderr, "%s: missing destination\n", argv[0]);
		return 1;
	case 2:
		if (single_link(argv[i], argv[i + 1])) {
			fprintf(stderr, "%s: %s\n",
				argv[0], argv[i]);
			return 1;
		}
		break;
	default:
		if (multi_link(argc - i, argv + i))
			return 1;
	}
	return 0;
}
