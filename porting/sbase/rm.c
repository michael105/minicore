/* See LICENSE file for copyright and license details. */
#include "fs.h"
#include "util.h"

static void
usage(void)
{
	eprintf("usage: %s [-f] [-Rr] file ...\n", argv0);
}

int
main(int argc, char *argv[])
{
	struct recursor r = { .fn = rm, .hist = NULL, .depth = 0, .maxdepth = 1,
	                      .follow = 'P', .flags = 0 };

	ARGBEGIN {
	case 'f':
		r.flags |= SILENT;
		break;
	case 'R':
	case 'r':
		r.maxdepth = 0;
		break;
	default:
		usage();
	} ARGEND

	if (!argc) {
		if (!(r.flags & SILENT))
			usage();
		else
			return 0;
	}

	for (; *argv; argc--, argv++)
		recurse(*argv, NULL, &r);

	return rm_status || recurse_status;
}
