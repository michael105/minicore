#define _DEFAULT_SOURCE

#include <sys/mman.h>
#include <fcntl.h>
#include <libgen.h>
#include <unistd.h>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define NUM_PATTERNS	44
#define PATTERN_LEN	3
#define RAND_SOURCE	"/dev/urandom"
#define DEFAULT_ITERS	3

static struct {
	const char *argv0;
	unsigned int iters;
	bool delete : 1;
	bool zero   : 1;
} opt;

static const char *patterns[] = {
	"\x00\x00\x00",
	"\x08\x88\x88",
	"\x11\x11\x11",
	"\x12\x49\x24",
	"\x19\x99\x99",
	"\x22\x22\x22",
	"\x24\x92\x49",
	"\x2a\xaa\xaa",
	"\x33\x33\x33",
	"\x36\xdb\x6d",
	"\x3b\xbb\xbb",
	"\x44\x44\x44",
	"\x49\x24\x92",
	"\x4c\xcc\xcc",
	"\x55\x55\x55",
	"\x5b\x6d\xb6",
	"\x5d\xdd\xdd",
	"\x66\x66\x66",
	"\x6d\xb6\xdb",
	"\x6e\xee\xee",
	"\x77\x77\x77",
	"\x7f\xff\xff",
	"\x80\x00\x00",
	"\x88\x88\x88",
	"\x91\x11\x11",
	"\x92\x49\x24",
	"\x99\x99\x99",
	"\xa2\x22\x22",
	"\xa4\x92\x49",
	"\xaa\xaa\xaa",
	"\xb3\x33\x33",
	"\xb6\xdb\x6d",
	"\xbb\xbb\xbb",
	"\xc4\x44\x44",
	"\xc9\x24\x92",
	"\xcc\xcc\xcc",
	"\xd5\x55\x55",
	"\xdb\x6d\xb6",
	"\xdd\xdd\xdd",
	"\xe6\x66\x66",
	"\xed\xb6\xdb",
	"\xee\xee\xee",
	"\xf7\x77\x77",
	"\xff\xff\xff",
	NULL
};

static int randfd;

static void fill_buffer(char *buf, size_t len)
{
	const char *pattern;
	unsigned int i, n, remain;

	pattern = patterns[random() % NUM_PATTERNS];
	if (!pattern) {
		if (read(randfd, buf, len) != (ssize_t)len) {
			exit(-1);
		}
		return;
	}

	n = len / PATTERN_LEN;
	remain = len % PATTERN_LEN;
	for (i = 0; i < n; i++) {
		memcpy(buf, pattern, PATTERN_LEN);
		buf += PATTERN_LEN;
	}
	memcpy(buf, pattern, remain);
}

static int delete(char *path)
{
	char *dir, *base, *base2;
	size_t len;
	int dirfd;

	base = basename(path);
	dir = dirname(path);

	dirfd = open(dir, O_RDONLY);
	if (dirfd < 0) {
		fprintf(stderr, "%s: %s: %s\n",
			opt.argv0, dir, strerror(errno));
		return 1;
	}
	len = strlen(base);
	base2 = malloc(len + 1);
	if (!base2) {
		fprintf(stderr, "%s: %s\n",
			opt.argv0, strerror(errno));
		return 1;
	}

	/* First rename to 0's */
	memset(base2, '0', len);
	base2[len] = '\0';
	if (renameat(dirfd, base, dirfd, base2)) {
		fprintf(stderr, "%s: %s/%s: %s\n",
			opt.argv0, dir, base, strerror(errno));
		return 1;
	}
	memset(base, '0', len);
	base[len] = '\0';

	/* Shrink filename size */
	while (len > 1) {
		len--;
		base2[len] = '\0';
		if (renameat(dirfd, base, dirfd, base2)) {
			fprintf(stderr, "%s: %s/%s: %s\n",
				opt.argv0, dir, base2, strerror(errno));
			return 1;
		}
		base[len] = '\0';
	}

	/* Finally delete */
	if (unlinkat(dirfd, base2, 0)) {
		fprintf(stderr, "%s: %s/%s: %s\n",
			opt.argv0, dir, base2, strerror(errno));
		return 1;
	}

	free(base2);
	if (close(dirfd)) {
		fprintf(stderr, "%s: %s: %s\n",
			opt.argv0, dir, strerror(errno));
		return 1;
	}
	return 0;
}

static int shred(char *path)
{
	void *buf;
	unsigned int i;
	off_t len;
	int fd;

	fd = open(path, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "%s: %s: %s\n",
			opt.argv0, path, strerror(errno));
		return 1;
	}
	len = lseek(fd, 0, SEEK_END);
	if (len < 0) {
		fprintf(stderr, "%s: %s: %s\n",
			opt.argv0, path, strerror(errno));
		return 1;
	} else if (len == 0) {
		return 0;
	}

	buf = mmap(NULL, len, PROT_WRITE, MAP_SHARED, fd, 0);
	if (buf == MAP_FAILED) {
		fprintf(stderr, "%s: mmap failed: %s\n",
			opt.argv0, strerror(errno));
		return 1;
	}

	for (i = 0; i < opt.iters; i++) {
		fill_buffer(buf, sizeof(buf));
		if (fsync(fd)) {
			fprintf(stderr, "%s: %s: %s\n",
				opt.argv0, path, strerror(errno));
			return 1;
		}
	}
	if (opt.zero) {
		memset(buf, 0, len);
	}
	if (munmap(buf, len)) {
		fprintf(stderr, "%s: unmap failed: %s\n",
			opt.argv0, strerror(errno));
		return 1;
	}
	if (close(fd)) {
		fprintf(stderr, "%s: %s: %s\n",
			opt.argv0, path, strerror(errno));
		return 1;
	}
	if (opt.delete) {
		return delete(path);
	}
	return 0;
}

static unsigned int parse_int(const char *str)
{
	char *ptr;
	long value;

	value = strtol(str, &ptr, 10);
	if (*ptr) {
		fprintf(stderr, "%s: invalid number: %s\n",
			opt.argv0, str);
		exit(1);
	}
	if (value < 0) {
		fprintf(stderr, "%s: number is negative: %s\n",
			opt.argv0, str);
		exit(1);
	}
	return value;
}

/* Usage: shred [-u] [-z] [-n=ITERATIONS] FILE... */
int main(int argc, char *argv[])
{
	int i;

	srandom(time(NULL));

	opt.argv0 = argv[0];
	opt.iters = DEFAULT_ITERS;
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			break;
		} else if (!strcmp(argv[i], "-u")) {
			opt.delete = 1;
		} else if (!strcmp(argv[i], "-z")) {
			opt.zero = 1;
		} else if (!strncmp(argv[i], "-n=", 3)) {
			opt.iters = parse_int(argv[i] + 3);
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

	randfd = open(RAND_SOURCE, O_RDONLY);
	if (randfd < 0) {
		fprintf(stderr, "%s: %s: %s\n",
			argv[0], RAND_SOURCE, strerror(errno));
		return 1;
	}
	for (; i < argc; i++) {
		if (shred(argv[i]))
			return 1;
	}
	if (close(randfd)) {
		fprintf(stderr, "%s: %s: %s\n",
			argv[0], RAND_SOURCE, strerror(errno));
		return 1;
	}

	return 0;
}
