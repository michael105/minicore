#define _XOPEN_SOURCE	500

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/vfs.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static struct {
	const char *argv0;
	const char *format;
	bool deref : 1;
} opt;

enum file_type {
	FILE_REGULAR,
	FILE_DIRECTORY,
	FILE_CHARACTER,
	FILE_BLOCK,
	FILE_FIFO,
	FILE_SYMLINK,
	FILE_SOCKET,
	FILE_UNKNOWN
};

static enum file_type get_file_type(mode_t mode)
{
	if (S_ISREG(mode))
		return FILE_REGULAR;
	else if (S_ISDIR(mode))
		return FILE_DIRECTORY;
	else if (S_ISCHR(mode))
		return FILE_CHARACTER;
	else if (S_ISBLK(mode))
		return FILE_BLOCK;
	else if (S_ISFIFO(mode))
		return FILE_FIFO;
	else if (S_ISLNK(mode))
		return FILE_SYMLINK;
	else if (S_ISSOCK(mode))
		return FILE_SOCKET;
	else
		return FILE_UNKNOWN;
}

static void print_format(const char *path, const struct stat *stbuf)
{
	puts("-- TODO --");
	(void)path;
	(void)stbuf;
	abort();
}

static char get_sticky(mode_t mode)
{
	if (mode & S_ISVTX) {
		if ((mode & S_IXUSR) &&
		    (mode & S_IXGRP) &&
		    (mode & S_IXOTH)) {
			return 't';
		} else {
			return 'T';
		}
	} else if (mode & S_IXOTH) {
		return 'x';
	} else {
		return '-';
	}
}

static void get_time(char *buf, size_t len, time_t t)
{
	const struct tm *tm;

	tm = localtime(&t);
	strftime(buf, len, "%Y-%m-%d %H:%M:%S %Z", tm);
}

static void print_statblock(const char *path, const struct stat *stbuf)
{
	const char *typename;
	struct passwd *pwd;
	struct group *grp;
	enum file_type type;
	char mode[11], deviceinf[21];
	char atime[32], mtime[32], ctime[32];

	pwd = getpwuid(stbuf->st_uid);
	grp = getgrgid(stbuf->st_gid);

	type = get_file_type(stbuf->st_mode);
	switch (type) {
	case FILE_REGULAR:
		typename = "regular file";
		mode[0] = '-';
		break;
	case FILE_DIRECTORY:
		typename = "directory";
		mode[0] = 'd';
		break;
	case FILE_CHARACTER:
		typename = "character special file";
		mode[0] = 'c';
		break;
	case FILE_BLOCK:
		typename = "block special file";
		mode[0] = 'b';
		break;
	case FILE_FIFO:
		typename = "fifo";
		mode[0] = 'p';
		break;
	case FILE_SYMLINK:
		typename = "symbolic link";
		mode[0] = 'l';
		break;
	case FILE_SOCKET:
		typename = "socket";
		mode[0] = 's';
		break;
	case FILE_UNKNOWN:
		typename = "unknown";
		mode[0] = '?';
	}
	if (type == FILE_CHARACTER || type == FILE_BLOCK) {
		sprintf(deviceinf, "Device type: %d,%d",
			major(stbuf->st_dev),
			minor(stbuf->st_dev));
	} else {
		deviceinf[0] = '\0';
	}

	mode[1] = (stbuf->st_mode & S_IRUSR) ? 'r' : '-';
	mode[2] = (stbuf->st_mode & S_IWUSR) ? 'w' : '-';
	mode[3] = (stbuf->st_mode & S_ISUID) ? 's' : ((stbuf->st_mode & S_IXUSR) ? 'x' : '-');
	mode[4] = (stbuf->st_mode & S_IRGRP) ? 'r' : '-';
	mode[5] = (stbuf->st_mode & S_IWGRP) ? 'w' : '-';
	mode[6] = (stbuf->st_mode & S_ISGID) ? 's' : ((stbuf->st_mode & S_IXGRP) ? 'x' : '-');
	mode[7] = (stbuf->st_mode & S_IROTH) ? 'r' : '-';
	mode[8] = (stbuf->st_mode & S_IWOTH) ? 'w' : '-';
	mode[9] = get_sticky(stbuf->st_mode);
	mode[10] = '\0';

	get_time(atime, sizeof(atime), stbuf->st_atime);
	get_time(mtime, sizeof(mtime), stbuf->st_mtime);
	get_time(ctime, sizeof(ctime), stbuf->st_ctime);

	printf("  File: %s\n"
	       "  Size: %ld\t\tBlocks: %ld\t   IO Block: %ld   %s\n"
	       "Device: %s/%ud\tInode: %lu\t   Links: %ld\t%s\n"
	       "Access: (%04o/%s)  Uid: (%5d/%8s)   Gid: (%5d/%8s)\n"
	       "Access: %s\n"
	       "Modify: %s\n"
	       "Change: %s\n"
	       " Birth: %s\n",
	       path,
	       (long)stbuf->st_size,
	       (long)stbuf->st_blocks,
	       (long)stbuf->st_blksize,
	       typename,
	       "???",
	       (unsigned)stbuf->st_dev,
	       (unsigned long)stbuf->st_ino,
	       (long)stbuf->st_nlink,
	       deviceinf,
	       stbuf->st_mode & 07777,
	       mode,
	       stbuf->st_uid,
	       (pwd) ? pwd->pw_name : "???",
	       stbuf->st_gid,
	       (grp) ? grp->gr_name : "???",
	       atime,
	       mtime,
	       ctime,
	       "-");
}

static int do_stat(const char *path, int (*stat_func)(const char *, struct stat *))
{
	struct stat stbuf;

	if (stat_func(path, &stbuf)) {
		return 1;
	}
	if (opt.format) {
		print_format(path, &stbuf);
	} else {
		print_statblock(path, &stbuf);
	}
	return 0;
}

/* stat [-L] [-c format] file... */
int main(int argc, char *argv[])
{
	int (*stat_func)(const char *, struct stat *);
	int ch, ret, i;

	opt.argv0 = argv[0];
	while ((ch = getopt(argc, argv, "Lc:")) != -1) {
		switch (ch) {
		case 'L':
			opt.deref = 1;
			break;
		case 'c':
			opt.format = optarg;
			break;
		case '?':
			return 1;
		default:
			abort();
		}
	}
	if (optind == argc) {
		fprintf(stderr, "%s: missing operand\n", argv[0]);
		return 1;
	}
	ret = 0;
	stat_func = (opt.deref) ? stat : lstat;
	for (i = optind; i < argc; i++) {
		if (do_stat(argv[i], stat_func)) {
			fprintf(stderr, "%s: %s: %s\n",
				argv[0], argv[i], strerror(errno));
			ret = 1;
		}
	}
	return ret;
}
