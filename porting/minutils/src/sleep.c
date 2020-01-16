#define _XOPEN_SOURCE	500

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define ISINF(x)	(((x) == INF) || ((x) == -INF))
#define ISNAN(x)	((x) != (x))

#define INF		(1.0/0.0)

static int parse_duration(struct timespec *duration, const char *str)
{
	char *ptr;
	time_t magnitude;
	int multiplier;
	double val;

	val = strtod(str, &ptr);
	if (!*str || errno || val < 0 || ISINF(val) || ISNAN(val))
		return -1;

	if (!*ptr || !strcmp(ptr, "s"))
		multiplier = 1;
	else if (!strcmp(ptr, "m"))
		multiplier = 60;
	else if (!strcmp(ptr, "h"))
		multiplier = 60 * 60;
	else if (!strcmp(ptr, "d"))
		multiplier = 60 * 60 * 24;
	else
		return -1;

	magnitude = (time_t)val;
	duration->tv_sec = magnitude * multiplier;
	duration->tv_nsec = (val - magnitude) * 1e9;
	return 0;
}

/* Usage: sleep SECONDS */
int main(int argc, const char *argv[])
{
	struct timespec duration;

	if (argc != 2) {
		fprintf(stderr, "usage: %s seconds\n", argv[0]);
		return 1;
	}
	if (parse_duration(&duration, argv[1])) {
		fprintf(stderr, "%s: invalid duration: %s\n",
			argv[0], argv[1]);
		return 1;
	}
	if (nanosleep(&duration, NULL)) {
		fprintf(stderr, "%s: unable to sleep: %s\n",
			argv[0], strerror(errno));
		return  1;
	}
	return 0;
}
