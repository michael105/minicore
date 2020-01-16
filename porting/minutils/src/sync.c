#define _XOPEN_SOURCE	500

#include <fcntl.h>
#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

/* Usage sync [PATH...] */
int main(int argc, const char *argv[])
{
	int i;

	if (argc == 1) {
		sync();
	} else for (i = 1; i < argc; i++) {
		int fd;

		fd = open(argv[i], O_WRONLY);
		if (fd < 0) {
			goto fail;
		}
		if (fsync(fd)) {
			goto fail;
		}
		if (close(fd)) {
			goto fail;
		}
	}

	return 0;
fail:
	fprintf(stderr, "%s: %s: %s\n",
		argv[0], argv[i], strerror(errno));
	return 1;
}
