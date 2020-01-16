#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static struct {
	const char *argv0;
	bool all : 1;
} opt;

static int list(const char *path, int label)
{
	static int notfirst;
	struct stat statbuf;
	struct dirent *ent;
	DIR *dh;

	if (stat(path, &statbuf)) {
		fprintf(stderr, "%s: unable to stat %s: %s\n",
			opt.argv0, path, strerror(errno));
		return 1;
	}
	if (!S_ISDIR(statbuf.st_mode)) {
		printf("%s\n", path);
		return 0;
	}
	if (label) {
		if (notfirst)
			putchar('\n');
		notfirst = 1;
		printf("%s:\n", path);
	}

	dh = opendir(path);
	if (!dh) {
		fprintf(stderr, "%s: %s: %s\n",
			opt.argv0, path, strerror(errno));
		return 1;
	}
	while ((ent = readdir(dh))) {
		if (ent->d_name[0] == '.' && !opt.all)
			continue;
		printf("%s\n", ent->d_name);
	}
	if (closedir(dh)) {
		fprintf(stderr, "%s: %s: %s\n",
			opt.argv0, path, strerror(errno));
		return 1;
	}
	return 0;
}

/* Usage: ls [-a] [PATH...] */
int main(int argc, const char *argv[])
{
	int i;

	opt.argv0 = argv[0];

	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			break;
		} else if (!strcmp(argv[i], "-a")) {
			opt.all = 1;
		} else {
			fprintf(stderr, "%s: invalid argument: %s\n",
				argv[0], argv[i]);
			return 1;
		}
	}

	if (i == argc) {
		if (list(".", 0))
			return 1;
	} else if (i == argc - 1) {
		if (list(argv[i], 0))
			return 1;
	} else for (; i < argc; i++) {
		if (list(argv[i], 1))
			return 1;
	}
	return 0;
}
