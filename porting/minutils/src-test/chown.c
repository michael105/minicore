#define _XOPEN_SOURCE	500

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct {
	const char *argv0;
	uid_t uid;
	gid_t gid;
	bool deref : 1;
} opt;

static void get_owner(char *user)
{
	char *group;
	struct {
		bool user   : 1;
		bool group  : 1;
		bool defgrp : 1;
	} s;

	group = strchr(user, ':');
	if (group == user) {
		s.user = 0;
		s.group = 1;
		s.defgrp = 0;
		group++;
	} else if (!group) {
		s.user = 1;
		s.group = 0;
		s.defgrp = 0;
	} else {
		*group = '\0';
		if (group[1] == '\0') {
			s.user = 1;
			s.group = 0;
			s.defgrp = 1;
		} else {
			s.user = 1;
			s.group = 1;
			s.defgrp = 0;
			group++;
		}
	}

	opt.uid = -1;
	opt.gid = -1;
	if (s.user) {
		const struct passwd *pwd;

		pwd = getpwnam(user);
		if (!pwd) {
			fprintf(stderr, "%s: invalid user: %s\n",
				opt.argv0, user);
			exit(1);
		}
		opt.uid = pwd->pw_uid;
		if (s.defgrp)
			opt.gid = pwd->pw_gid;
	}
	if (s.group) {
		const struct group *grp;

		grp = getgrnam(group);
		if (!grp) {
			fprintf(stderr, "%s: invalid group: %s\n",
				opt.argv0, group);
			exit(1);
		}
		opt.gid = grp->gr_gid;
	}
}

static void do_chown(int count, char **files)
{
	int (*chown_func)(const char *, uid_t, gid_t);
	int i, ret;

	chown_func = (opt.deref) ? chown : lchown;
	ret = 0;
	for (i = 0; i < count; i++) {
		if (chown_func(files[i], opt.uid, opt.gid)) {
			fprintf(stderr, "%s: %s: %s\n",
				opt.argv0, files[i], strerror(errno));
			ret = 1;
		}
	}
	exit(ret);
}

/* Usage chown [-h] [owner][:[group]] FILE... */
int main(int argc, char *argv[])
{
	int i;

	opt.argv0 = argv[0];
	opt.deref = 1;
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			break;
		} else if (!strcmp(argv[i], "-h")) {
			opt.deref = 0;
		} else {
			fprintf(stderr, "%s: invalid argument: %s\n",
				argv[0], strerror(errno));
			return 1;
		}
	}
	if (i == argc) {
		fprintf(stderr, "%s: missing operand\n", argv[0]);
		return 1;
	}
	get_owner(argv[i++]);
	if (i == argc) {
		fprintf(stderr, "%s: missing operand\n", argv[0]);
		return 1;
	}
	do_chown(argc - i, argv + i);
	return 0;
}
