#include <fcntl.h>
#include <unistd.h>
#include <utime.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static const char *argv0;

static int touch(const char *path)
{
	struct utimbuf new_times;
	time_t new_time;
	int fd;

	fd = open(path, O_CREAT | O_RDONLY, 0666);
	if (fd < 0) {
		fprintf(stderr, "%s: %s: %s\n",
			argv0, path, strerror(errno));
		return 1;
	}
	if (close(fd)) {
		fprintf(stderr, "%s: %s: %s\n",
			argv0, path, strerror(errno));
		return 1;
	}

	new_time = time(NULL);
	new_times.actime = new_time;
	new_times.modtime = new_time;

	if (utime(path, &new_times) < 0) {
		return 1;
	}
	return 0;
}

/* Usage: touch FILE... */
int main(int argc, char *argv[])
{
	int i;

	argv0 = argv[0];
	if (argc == 1) {
		fprintf(stderr, "%s: missing operand\n", argv[0]);
		return 1;
	}
	for (i = 1; i < argc; i++) {
		if (touch(argv[i]))
			return 1;
	}
	return 0;
}
