#define _XOPEN_SOURCE	500

#include <sys/types.h>
#include <signal.h>
#include <strings.h>
#include <unistd.h>

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define ISINF(x)	(((x) == INF) || ((x) == -INF))
#define ISNAN(x)	((x) != (x))
#define UNUSED(x)	((void)(x))

#define INF		(1.0/0.0)

static struct {
	const char *argv0;
	struct timespec duration;
	pid_t pid;
} opt;

static void on_sigchld(int signum)
{
	UNUSED(signum);
	assert(signum == SIGCHLD);

	exit(0);
}

static int parse_duration(const char *str)
{
	char *ptr;
	double amount;
	time_t secs;
	int mult;

	amount = strtod(str, &ptr);
	if (!*str || errno || ISINF(amount) || ISNAN(amount)) {
		fprintf(stderr, "%s: invalid duration: %s\n",
			opt.argv0, ptr);
		return -1;
	}

	if (!strcasecmp(ptr, "s") || *ptr) {
		mult = 1;
	} else if (!strcasecmp(ptr, "m")) {
		mult = 60;
	} else if (!strcasecmp(ptr, "h")) {
		mult = 60 * 60;
	} else if (!strcasecmp(ptr, "d")) {
		mult = 60 * 60 * 24;
	} else {
		fprintf(stderr, "%s: invalid suffix: %s\n",
			opt.argv0, ptr);
		return -1;
	}

	secs = (time_t)amount;
	opt.duration.tv_sec = secs * mult;
	opt.duration.tv_nsec = (amount - secs) * 1e9;
	return 0;
}

static int spawn(char *argv[])
{
	if ((opt.pid = fork()) < 0) {
		fprintf(stderr, "%s: unable to fork: %s\n",
			opt.argv0, strerror(errno));
		return -1;
	}
	if (!opt.pid) {
		execvp(argv[0], argv);
		fprintf(stderr, "%s: %s: unable to exec: %s\n",
			opt.argv0, argv[0], strerror(errno));
		_exit(-1);
	}
	return 0;
}

static void xkill(int signum)
{
	if (kill(opt.pid, signum)) {
		fprintf(stderr, "%s: unable to kill child: %s\n",
			opt.argv0, strerror(errno));
		exit(1);
	}
}

/* Usage: timeout DURATION COMMAND [ARGS]... */
int main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr, "usage: %s DURATION COMMAND [ARGS]...\n", argv[0]);
		return 1;
	}

	signal(SIGCHLD, &on_sigchld);
	if (parse_duration(argv[1]))
		return 1;
	if (spawn(argv + 2))
		return 1;

	/* Wait then terminate */
	if (nanosleep(&opt.duration, NULL)) {
		assert(errno == EINTR);
		xkill(SIGINT);
	}
	xkill(SIGTERM);

	/* Finish it off it's still around */
	opt.duration.tv_sec = 5;
	nanosleep(&opt.duration, NULL);
	xkill(SIGKILL);
	return 0;
}
