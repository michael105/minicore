#define _POSIX_C_SOURCE		200809L

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <libgen.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct {
	const char *argv0;
} opt;

static int copy_file(const char *source, int destfd, const char *destfn)
{
	char buf[4096];
	ssize_t len;
	int ifd, ofd;

	ifd = open(source, O_RDONLY);
	if (ifd < 0)
		return 1;
	ofd = openat(destfd, destfn, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (ofd < 0)
		return 1;

	do {
		len = read(ifd, buf, sizeof(buf));
		if (len < 0)
			return 1;
		else if (len == 0)
			break;
		if (write(ofd, buf, len) != len)
			return 1;
	} while (len);
	close(ifd);
	close(ofd);
	if (unlink(source))
		return 1;
	return 0;
}

static int copy_file2(const char *source, const char *dest)
{
	char *copy, *destfn;
	int destfd, ret;

	destfd = open(dest, O_RDONLY);
	if (destfd < 0)
		return 1;

	copy = strdup(source);
	if (!copy)
		return 1;
	destfn = basename(copy);
	ret = copy_file(source, destfd, destfn);
	free(copy);
	close(destfd);
	return ret;
}

static int multi_move(int len, const char *files[], const char *dest)
{
	struct stat stbuf;
	int destfd, i;

	destfd = open(dest, O_RDONLY);
	if (destfd < 0) {
		fprintf(stderr, "%s: %s: %s\n",
			opt.argv0, dest, strerror(errno));
		return 1;
	}
	if (fstat(destfd, &stbuf) || !S_ISDIR(stbuf.st_mode)) {
		fprintf(stderr, "%s: %s: not a directory\n",
			opt.argv0, dest);
		return 1;
	}
	for (i = 0; i < len; i++) {
		char *copy, *destfn;

		copy = strdup(files[i]);
		if (!copy) {
			fprintf(stderr, "%s: %s\n",
				opt.argv0, strerror(errno));
			return 1;
		}
		destfn = basename(copy);
		if (renameat(AT_FDCWD, files[i], destfd, destfn)) {
			if (errno == EXDEV && !copy_file(files[i], destfd, destfn)) {
				free(copy);
				continue;
			}
			fprintf(stderr, "%s: %s: %s\n",
				opt.argv0, files[i], strerror(errno));
			return 1;
		}
		free(copy);
	}
	close(destfd);
	return 0;
}

/* Usage: mv FILES... DEST */
int main(int argc, const char *argv[])
{
	opt.argv0 = argv[0];

	if (argc == 2) {
		fprintf(stderr, "%s: missing destination\n", argv[0]);
		return 1;
	} else if (argc == 3) {
		const char *source, *dest;

		source = argv[1];
		dest = argv[2];
		if (rename(source, dest)) {
			if (errno == EXDEV && !copy_file2(source, dest))
				return 0;
			fprintf(stderr, "%s: %s\n",
				argv[0], strerror(errno));
			return 1;
		}
	} else {
		return multi_move(argc - 2, argv + 1, argv[argc - 1]);
	}
	return 0;
}
