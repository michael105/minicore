#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

static int mmap_copy(int ifd, int ofd)
{
	off_t len;
	void *ptr;

	len = lseek(ifd, 0, SEEK_END);
	if (len <= 0)
		return 1;
	ptr = mmap(NULL, len, PROT_READ, MAP_PRIVATE, ifd, 0);
	if (ptr == MAP_FAILED)
		return 1;
	if (write(ofd, ptr, len) != len)
		return 1;
	if (munmap(ptr, len))
		return 1;
	return 0;
}

static int buffer_copy(int ifd, int ofd)
{
	char buf[4096];
	ssize_t len;

	do {
		len = read(ifd, buf, sizeof(buf));
		if (len < 0)
			return 1;
		else if (len == 0)
			return 0;
		if (write(ofd, buf, len) != len)
			return 1;
	} while (len);
	return 0;
}

/* Usage: cp FILES... DEST */
int main(int argc, char *argv[])
{
	int ifd, ofd, ret;

	if (argc != 3) {
		fprintf(stderr, "usage: %s input output\n", argv[0]);
		return 1;
	}

	ifd = open(argv[1], O_RDONLY);
	if (ifd < 0) {
		fprintf(stderr, "%s: %s: %s\n",
			argv[0], argv[1], strerror(errno));
		return 1;
	}
	ofd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (ofd < 0) {
		fprintf(stderr, "%s: %s: %s\n",
			argv[0], argv[2], strerror(errno));
		return 1;
	}

	ret = mmap_copy(ifd, ofd);
	if (ret)
		ret = buffer_copy(ifd, ofd);
	if (close(ifd) || close(ofd))
		return 1;
	return ret;
}
