#include <sys/types.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>

static int is_octal(mode_t *mode, const char *str)
{
	size_t i;

	for (i = 0; str[i]; i++) {
		if ('0' > str[i] || str[i] > '7')
			return 0;

		*mode <<= 3;
		*mode += str[i] - '0';
	}
	return 1;
}

static mode_t parse_mode(const char *str)
{
	struct {
		bool user  : 1;
		bool group : 1;
		bool other : 1;
	} flags;
	enum {
		PERM_REMOVE,
		PERM_ADD,
		PERM_SET
	} perm;
	mode_t mode = 0;
	size_t i;

	if (is_octal(&mode, str))
		return mode;

	flags.user = 0;
	flags.group = 0;
	flags.other = 0;

	for (i = 0; str[i]; i++) {
		switch (str[0]) {
		case 'u':
			flags.user = 1;
			break;
		case 'g':
			flags.group = 1;
			break;
		case 'o':
			flags.other = 1;
			break;
		case 'a':
			flags.user = 1;
			flags.group = 1;
			flags.other = 1;
			break;
		default:
			/* TODO */
			break;
		}
	}
}

/* Usage: chmod MODE FILE... */
int main(int argc, const char *argv[])
{
	mode_t mode;
	int i;

	if (argc < 3) {
		fprintf(stderr, "usage: %s MODE FILE...\n", argv[0]);
		return 1;
	}

	mode = parse_mode(argv[1]);

	return 0;
}
