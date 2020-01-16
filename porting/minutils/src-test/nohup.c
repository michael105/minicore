#define _XOPEN_SOURCE	500

#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>
#include <signal.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct {
	const char *argv0;
} opt;

static void block_hup(void)
{
	sigset_t set;

	sigemptyset(&set);
	sigaddset(&set, SIGHUP);

	if (sigprocmask(SIG_BLOCK, &set, NULL)) {
		fprintf(stderr, "%s: unable to block SIGHUP: %s\n",
			opt.argv0, strerror(errno));
		exit(1);
	}
}

/*
 * Open "nohup.out", and if that fails,
 * try "$HOME/nohup.out".
 */
static int open_nohup(void)
{
	const struct passwd *pwd;
	char *path;
	int fd;

	fd = open("nohup.out", O_CREAT | O_APPEND | O_WRONLY, 0600);
	if (fd >= 0)
		return fd;

	pwd = getpwuid(geteuid());
	path = malloc(strlen(pwd->pw_name) + strlen("/nohup.out") + 1);
	if (!path) {
		fprintf(stderr, "%s: unable to allocate buffer for path: %s\n",
			opt.argv0, strerror(errno));
		exit(1);
	}

	sprintf(path, "%s/nohup.out", pwd->pw_name);
	fd = open(path, O_CREAT | O_APPEND | O_WRONLY, 0600);
	if (fd < 0) {
		fprintf(stderr, "%s: unable to open 'nohup.out': %s\n",
			opt.argv0, strerror(errno));
		exit(1);
	}
	free(path);
	return fd;
}

/* Usage: nohup COMMAND [ARG]... */
int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "usage: %s COMMAND [ARG]...\n", argv[0]);
		return 1;
	}

	opt.argv0 = argv[0];

	puts("ignoring input and appending output to 'nohup.out'");
	block_hup();

	if (isatty(STDIN_FILENO)) {
		int fd;

		fd = open("/dev/null", O_RDONLY);
		if (fd < 0) {
			fprintf(stderr, "%s: unable to open null file: %s\n",
				argv[0], strerror(errno));
			exit(1);
		}
		if (dup2(fd, STDIN_FILENO) < 0) {
			fprintf(stderr, "%s: unable to overwrite stdin: %s\n",
				argv[0], strerror(errno));
			exit(1);
		}
		close(fd);
	}
	if (isatty(STDOUT_FILENO)) {
		int fd;

		fd = open_nohup();
		if (dup2(fd, STDOUT_FILENO) < 0) {
			fprintf(stderr, "%s: unable to overwrite stdout: %s\n",
				argv[0], strerror(errno));
			return 1;
		}
		close(fd);
	}
	if (isatty(STDERR_FILENO)) {
		if (dup2(STDOUT_FILENO, STDERR_FILENO) < 0) {
			fprintf(stderr, "%s: unable to overwrite stderr: %s\n",
				argv[0], strerror(errno));
			return 1;
		}
	}

	/* Execute program */
	execvp(argv[1], &argv[1]);
	fprintf(stderr, "%s: %s: unable to exec: %s\n",
		argv[0], argv[1], strerror(errno));
	return 1;
}
