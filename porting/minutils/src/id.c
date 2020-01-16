#define _DEFAULT_SOURCE

#include <sys/types.h>
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
	bool group  : 1;
	bool allgrp : 1;
	bool name   : 1;
	bool real   : 1;
	bool user   : 1;
} opt;

static void print_groups(const struct passwd *pwd)
{
	if (!opt.name)
		printf("%d", pwd->pw_gid);
	else
		fputs(pwd->pw_name, stdout);
	if (opt.allgrp) {
		gid_t groups[NGROUPS_MAX + 1];
		int count, i;

		count = getgroups(sizeof(groups), groups);
		if (count < 0) {
			fprintf(stderr, "%s: %s\n",
				opt.argv0, strerror(errno));
			exit(1);
		}
		for (i = 0; i < count; i++) {
			struct group *grp;

			if (groups[i] == pwd->pw_gid)
				continue;
			putchar(' ');
			grp = getgrgid(groups[i]);
			if (!opt.name || !grp)
				printf("%d", groups[i]);
			else
				fputs(grp->gr_name, stdout);
		}
	}
	putchar('\n');
}

static void ids(void)
{
	const struct passwd *pwd;
	uid_t uid;

	uid = (opt.real) ? getuid() : geteuid();
	pwd = getpwuid(uid);
	if (!pwd) {
		fprintf(stderr, "%s: %s\n",
			opt.argv0, strerror(errno));
		exit(1);
	}

	if (opt.user) {
		if (!opt.name)
			printf("%d\n", uid);
		else
			puts(pwd->pw_name);
	}
	if (opt.group)
		print_groups(pwd);
}

static void default_ids(void)
{
	const struct passwd *pwd;
	const struct group *grp;
	gid_t groups[NGROUPS_MAX + 1];
	uid_t uid;
	int count, i;

	uid = (opt.real) ? getuid() : geteuid();
	pwd = getpwuid(uid);
	if (!pwd) {
		fprintf(stderr, "%s: %s\n",
			opt.argv0, strerror(errno));
		exit(1);
	}

	printf("uid=%d(%s) gid=%d",
	       uid,
	       pwd->pw_name,
	       pwd->pw_gid);
	grp = getgrgid(pwd->pw_gid);
	if (grp)
		printf("(%s)", grp->gr_name);
	putchar(' ');
	count = getgroups(sizeof(groups), groups);
	if (count < 0) {
		fprintf(stderr, "%s: %s\n",
			opt.argv0, strerror(errno));
		exit(1);
	}
	for (i = 0; i < count; i++) {
		grp = getgrgid(groups[i]);
		printf("%d", groups[i]);
		if (grp)
			printf("(%s)", grp->gr_name);
		if (i < count - 1)
			putchar(',');
	}
	putchar('\n');
}

/* Usage: id [-a] [-g] [-G] [-n] [-r] [-u] */
int main(int argc, char *argv[])
{
	int ch;

	opt.argv0 = argv[0];
	while ((ch = getopt(argc, argv, "agGnru")) != -1) {
		switch (ch) {
		case 'a':
			/* ignored */
			break;
		case 'G':
			opt.allgrp = 1;
			/* FALLTHROUGH */
		case 'g':
			opt.group = 1;
			break;
		case 'n':
			opt.name = 1;
			break;
		case 'r':
			opt.real = 1;
			break;
		case 'u':
			opt.user = 1;
			break;
		case '?':
			return 1;
		default:
			abort();
		}
	}
	if (optind < argc) {
		fprintf(stderr, "%s: extra operand: %s\n",
			argv[0], argv[optind]);
		return 1;
	}
	if (opt.user && opt.group) {
		fprintf(stderr, "%s: user option is exclusive\n", argv[0]);
		return 1;
	}
	if (!(opt.user || opt.group) && (opt.name || opt.real)) {
		fprintf(stderr, "%s: no names to print\n", argv[0]);
		return 1;
	}

	if (opt.user || opt.group)
		ids();
	else
		default_ids();
	return 0;
}
