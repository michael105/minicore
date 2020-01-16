/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	eprintf("usage: %s [name]\n", argv0);
}

int
main(int argc, char *argv[])
{
	char host[HOST_NAME_MAX + 1];

	argv0 = *argv, argv0 ? (argc--, argv++) : (void *)0;

	if (!argc) {
		if (gethostname(host, sizeof(host)) < 0)
			eprintf("gethostname:");
		puts(host);
	} else if (argc == 1) {
		if (sethostname(argv[0], strlen(argv[0])) < 0)
			eprintf("sethostname:");
	} else {
		usage();
	}

	return fshut(stdout, "<stdout>");
}
