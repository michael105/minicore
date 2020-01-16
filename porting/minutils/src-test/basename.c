#include <libgen.h>

#include <stdio.h>
#include <string.h>

static void remove_suffix(char *base, const char *suffix)
{
	size_t baselen, suflen;
	char *ptr;

	baselen = strlen(base);
	suflen = strlen(suffix);

	ptr = base + baselen - suflen;
	if (!strcmp(ptr, suffix))
		*ptr = '\0';
}

/* Usage: basename PATH [SUFFIX] */
int main(int argc, char *argv[])
{
	const char *suffix;
	char *base;

	switch (argc) {
	case 2:
		suffix = NULL;
		break;
	case 3:
		suffix = argv[2];
		break;
	default:
		fputs("basename: missing operand",stderr );
		return 1;
	}
	base = basename(argv[1]);
	if (suffix)
		remove_suffix(base, suffix);

	puts(base);
	return 0;
}
