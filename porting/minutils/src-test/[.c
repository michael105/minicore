#define _XOPEN_SOURCE	500

#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct test_action {
	enum test_type {
		/* File checks */
		TEST_BLOCK,
		TEST_CHAR,
		TEST_DIR,
		TEST_EXISTS,
		TEST_FILE,
		TEST_SGID,
		TEST_GROUP,
		TEST_SYMLINK,
		TEST_STICKY,
		TEST_OWNER,
		TEST_FIFO,
		TEST_READ,
		TEST_SIZE,
		TEST_SOCKET,
		TEST_TTY,
		TEST_SUID,
		TEST_WRITE,
		TEST_EXE,

		/* File comparisons */
		TEST_SAMEFILE,
		TEST_NEWER,
		TEST_OLDER,

		/* String checks */
		TEST_STR_ZERO,
		TEST_STR_NONZERO,

		/* String comparisons */
		TEST_STR_EQ,
		TEST_STR_NE,

		/* Integer comparisons */
		TEST_INT_EQ,
		TEST_INT_NE,
		TEST_INT_GT,
		TEST_INT_GE,
		TEST_INT_LT,
		TEST_INT_LE,

		/* Invalid */
		TEST_INVALID
	} type;
	const char *first;
	const char *second;
};

/*
 * Get the appropriate action from the test flag.
 * Doesn't print any errors.
 */
static enum test_type get_test_type_short(const char *flag)
{
	if (flag[0] != '-') {
		if      (!strcmp(flag, "=="))
			return TEST_STR_EQ;
		else if (!strcmp(flag, "!="))
			return TEST_STR_NE;
		else
			return TEST_INVALID;
	}

	switch (flag[1]) {
	/* File types */
	case 'b':
		return TEST_BLOCK;
	case 'c':
		return TEST_CHAR;
	case 'd':
		return TEST_DIR;
	case 'e':
		return TEST_EXISTS;
	case 'f':
		return TEST_FILE;
	case 'g':
		return TEST_SGID;
	case 'G':
		return TEST_GROUP;
	case 'h':
	case 'L':
		return TEST_SYMLINK;
	case 'k':
		return TEST_STICKY;
	case 'O':
		return TEST_OWNER;
	case 'p':
		return TEST_FIFO;
	case 'r':
		return TEST_READ;
	case 's':
		return TEST_SIZE;
	case 'S':
		return TEST_SOCKET;
	case 't':
		return TEST_TTY;
	case 'u':
		return TEST_SUID;
	case 'w':
		return TEST_WRITE;
	case 'x':
		return TEST_EXE;

	/* Strings */
	case 'n':
		return TEST_STR_NONZERO;
	case 'z':
		return TEST_STR_ZERO;
	default:
		return TEST_INVALID;
	}
}

static enum test_type get_test_type_long(const char *flag)
{
	if      (!strcmp(flag, "-eq")) return TEST_INT_EQ;
	else if (!strcmp(flag, "-ne")) return TEST_INT_NE;
	else if (!strcmp(flag, "-gt")) return TEST_INT_GT;
	else if (!strcmp(flag, "-ge")) return TEST_INT_GE;
	else if (!strcmp(flag, "-lt")) return TEST_INT_LT;
	else if (!strcmp(flag, "-le")) return TEST_INT_LE;
	else if (!strcmp(flag, "-ef")) return TEST_SAMEFILE;
	else if (!strcmp(flag, "-nt")) return TEST_NEWER;
	else if (!strcmp(flag, "-ot")) return TEST_OLDER;
	else                           return TEST_INVALID;
}

static enum test_type get_test_type(const char *flag)
{
	switch (strlen(flag)) {
	case 2:
		return get_test_type_short(flag);
	case 3:
		return get_test_type_long(flag);
	case 1:
		if (flag[0] == '=')
			return TEST_STR_EQ;
		/* FALLTHROUGH */
	default:
		return TEST_INVALID;
	}
}

/* File operations */
static int test_stat(enum test_type type, const char *path)
{
	struct stat stbuf;
	int (*func)(const char *, struct stat *);

	func = (type == TEST_SYMLINK) ? lstat : stat;

	if (func(path, &stbuf))
		return 0;

	switch (type) {
	/* Existence */
	case TEST_EXISTS:
		return 1;

	/* File type */
	case TEST_BLOCK:
		return S_ISBLK(stbuf.st_mode);
	case TEST_CHAR:
		return S_ISCHR(stbuf.st_mode);
	case TEST_DIR:
		return S_ISDIR(stbuf.st_mode);
	case TEST_FILE:
		return S_ISREG(stbuf.st_mode);
	case TEST_FIFO:
		return S_ISFIFO(stbuf.st_mode);
	case TEST_SOCKET:
		return S_ISSOCK(stbuf.st_mode);
	case TEST_SYMLINK:
		return S_ISLNK(stbuf.st_mode);

	/* Ownership */
	case TEST_OWNER:
		return stbuf.st_uid == geteuid();
	case TEST_GROUP:
		return stbuf.st_gid == getegid();

	/* File size */
	case TEST_SIZE:
		return stbuf.st_size > 0;

	/* Special permissions */
	case TEST_SUID:
		return stbuf.st_mode & S_ISUID;
	case TEST_SGID:
		return stbuf.st_mode & S_ISGID;
	case TEST_STICKY:
		return stbuf.st_mode & S_ISVTX;

	/* Access checks */
	case TEST_READ:
		return !access(path, R_OK);
	case TEST_WRITE:
		return !access(path, W_OK);
	case TEST_EXE:
		return !access(path, X_OK);

	/* Error */
	default:
		abort();
	}

	return -1;
}

static int test_tty(const char *fdstr)
{
	char *ptr;
	int fd;

	fd = strtol(fdstr, &ptr, 0);
	if (!*fdstr || *ptr)
		return 0;
	return isatty(fd);
}

static int file_cmp(const struct test_action *act)
{
	struct stat stbuf_a, stbuf_b;

	if (stat(act->first, &stbuf_a))
		return 0;
	if (stat(act->second, &stbuf_b))
		return 0;

	switch (act->type) {
	case TEST_SAMEFILE:
		return stbuf_a.st_ino == stbuf_b.st_ino &&
			stbuf_a.st_dev == stbuf_b.st_dev;
	case TEST_NEWER:
		return stbuf_a.st_mtime > stbuf_b.st_mtime;
	case TEST_OLDER:
		return stbuf_a.st_mtime < stbuf_b.st_mtime;
	default:
		abort();
	}
}

/* Integer operations */
static int get_ints(long *x,
		    long *y,
		    const struct test_action *act)
{
	char *ptr;

	*x = strtol(act->first, &ptr, 0);
	if (!*act->first || *ptr)
		return -1;

	*y = strtol(act->second, &ptr, 0);
	if (!*act->second || *ptr)
		return -1;

	return 0;
}

static int int_cmp(const struct test_action *act)
{
	long x, y;

	switch (act->type) {
	case TEST_INT_EQ:
		if (get_ints(&x, &y, act))
			return -1;
		return x == y;
	case TEST_INT_NE:
		if (get_ints(&x, &y, act))
			return -1;
		return x != y;
	case TEST_INT_GT:
		if (get_ints(&x, &y, act))
			return -1;
		return x > y;
	case TEST_INT_GE:
		if (get_ints(&x, &y, act))
			return -1;
		return x >= y;
	case TEST_INT_LT:
		if (get_ints(&x, &y, act))
			return -1;
		return x < y;
	case TEST_INT_LE:
		if (get_ints(&x, &y, act))
			return -1;
		return x <= y;
	default:
		abort();
	}
}

/*
 * Runs a struct test_action and returns the result.
 * The value returned is a _true_ boolean, meaning
 * 0 for 0 and nonzero for 1.
 */
static int run_action(const struct test_action *act)
{
	switch (act->type) {
	case TEST_BLOCK:
	case TEST_CHAR:
	case TEST_DIR:
	case TEST_EXISTS:
	case TEST_FILE:
	case TEST_SGID:
	case TEST_GROUP:
	case TEST_SYMLINK:
	case TEST_STICKY:
	case TEST_OWNER:
	case TEST_FIFO:
	case TEST_READ:
	case TEST_SIZE:
	case TEST_SOCKET:
	case TEST_SUID:
	case TEST_WRITE:
	case TEST_EXE:
		return test_stat(act->type, act->first);
	case TEST_TTY:
		return test_tty(act->first);
	case TEST_SAMEFILE:
	case TEST_NEWER:
	case TEST_OLDER:
		return file_cmp(act);
	case TEST_STR_ZERO:
		return !act->first[0];
	case TEST_STR_NONZERO:
		return act->first[0];
	case TEST_STR_EQ:
		return !strcmp(act->first, act->second);
	case TEST_STR_NE:
		return strcmp(act->first, act->second);
	case TEST_INT_EQ:
	case TEST_INT_NE:
	case TEST_INT_GT:
	case TEST_INT_GE:
	case TEST_INT_LT:
	case TEST_INT_LE:
		return int_cmp(act);
	case TEST_INVALID:
	default:
		abort();
	}
}

static int is_bracket(const char *argv0)
{
	char *buf;
	int ret;

	buf = strdup(argv0);
	if (!buf) {
		perror("unable to allocate");
		exit(-1);
	}

	ret = !strcmp(basename(buf), "[");
	free(buf);
	return ret;
}

int main(int argc, char *argv[])
{
	struct test_action act;
	const char *argv0;
	int invert, ret;

	/* General checks */
	argv0 = argv[0];
	if (is_bracket(argv0)) {
		if (strcmp(argv[argc - 1], "]")) {
			fprintf(stderr, "[: missing `]'\n");
			return -1;
		}
		argc--;
	}
	if (argc > 1 && !strcmp(argv[1], "!")) {
		invert = 1;
		argv++;
		argc--;
	} else {
		invert = 0;
	}

	switch (argc) {
	/* [ ] */
	case 1:
		fprintf(stderr, "%s: missing operand\n",
			argv0);
		return -1;
	/* [ x ] */
	case 2:
		/* Default action is "-n" */
		act.type = TEST_STR_NONZERO;
		act.first = argv[1];
		break;
	/* [ -f x ] */
	case 3:
		act.type = get_test_type(argv[1]);
		act.first = argv[2];
		act.second = "";
		if (act.type == TEST_INVALID) {
			fprintf(stderr, "%s: unknown flag: '%s'\n",
				argv0, argv[1]);
			return -1;
		}
		break;
	/* [ x -eq y ] */
	case 4:
		act.type = get_test_type(argv[2]);
		act.first = argv[1];
		act.second = argv[3];
		if (act.type == TEST_INVALID) {
			fprintf(stderr, "%s: unknown flag: '%s'",
				argv0, argv[2]);
			return -1;
		}
		break;
	/* Even more... */
	default:
		fprintf(stderr, "%s: too many operands\n", argv0);
		return -1;
	}


	/*
	 * We can optimize out a branch by using XOR here.
	 * Since we know both "ret" and "invert" is
	 * either 0 or 1, we can use this expression to
	 * flip the result.
	 */
	ret = !run_action(&act);
	return ret ^ invert;
}
