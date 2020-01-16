/* See LICENSE file for copyright and license details. */
#include <string.h>
#include <stdio.h>

#include "utf.h"
#include "util.h"

static int    lflag = 0;
static int    wflag = 0;
static char   cmode = 0;
static size_t tc = 0, tl = 0, tw = 0;

static void
output(const char *str, size_t nc, size_t nl, size_t nw)
{
	int noflags = !cmode && !lflag && !wflag;
	int first = 1;

	if (lflag || noflags) {
		if (!first)
			putchar(' ');
		printf("%*.1zu", first ? (first = 0) : 6, nl);
	}
	if (wflag || noflags) {
		if (!first)
			putchar(' ');
		printf("%*.1zu", first ? (first = 0) : 6, nw);
	}
	if (cmode || noflags) {
		if (!first)
			putchar(' ');
		printf("%*.1zu", first ? (first = 0) : 6, nc);
	}
	if (str)
		printf(" %s", str);
	putchar('\n');
}

static void
wc(FILE *fp, const char *str)
{
	int word = 0, rlen;
	Rune c;
	size_t nc = 0, nl = 0, nw = 0;

	while ((rlen = efgetrune(&c, fp, str))) {
		nc += (cmode == 'c' || !cmode) ? rlen : (c != Runeerror);
		if (c == '\n')
			nl++;
		if (!isspacerune(c))
			word = 1;
		else if (word) {
			word = 0;
			nw++;
		}
	}
	if (word)
		nw++;
	tc += nc;
	tl += nl;
	tw += nw;
	output(str, nc, nl, nw);
}

static void
usage(void)
{
	eprintf("usage: %s [-c | -m] [-lw] [file ...]\n", argv0);
}

int
main(int argc, char *argv[])
{
	FILE *fp;
	int many;
	int ret = 0;

	ARGBEGIN {
	case 'c':
		cmode = 'c';
		break;
	case 'm':
		cmode = 'm';
		break;
	case 'l':
		lflag = 1;
		break;
	case 'w':
		wflag = 1;
		break;
	default:
		usage();
	} ARGEND

	if (!argc) {
		wc(stdin, NULL);
	} else {
		for (many = (argc > 1); *argv; argc--, argv++) {
			if (!strcmp(*argv, "-")) {
				*argv = "<stdin>";
				fp = stdin;
			} else if (!(fp = fopen(*argv, "r"))) {
				weprintf("fopen %s:", *argv);
				ret = 1;
				continue;
			}
			wc(fp, *argv);
			if (fp != stdin && fshut(fp, *argv))
				ret = 1;
		}
		if (many)
			output("total", tc, tl, tw);
	}

	ret |= fshut(stdin, "<stdin>") | fshut(stdout, "<stdout>");

	return ret;
}
