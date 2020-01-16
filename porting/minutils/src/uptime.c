#define _XOPEN_SOURCE	500

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define UNUSED(x)	((void)(x))

static const char *argv0;

static int get_uptime(struct timespec *ts, uintmax_t *now);

#if defined(__linux__)
static int get_uptime(struct timespec *ts, uintmax_t *now)
{
	if (clock_gettime(CLOCK_BOOTTIME, ts) < 0) {
		fprintf(stderr, "%s: clock_gettime() failed: %s\n",
			argv0, strerror(errno));
		return -1;
	}

	*now = (uintmax_t)ts->tv_sec;
	return 0;
}
#elif defined(__FreeBSD__) || defined(__OpenBSD__)
# include <sys/types.h>
# include <sys/sysctl.h>

static int get_uptime(struct timespec *ts, uintmax_t *now)
{
	const size_t len = sizeof(*ts);
	int mib[] = { CTL_KERN, KERN_BOOTTIME };
	time_t tm;

	if (sysctl(mib, 2, ts, &len, NULL, 0)) {
		fprintf(stderr, "%s: sysctl({CTL_KERN, KERN_BOOTTIME}, ...) failed\n",
			argv0);
		return -1;
	}
	*now = (uintmax_t)time(NULL) - (uintmax_t)ts->tv_sec;
	return 0;
}
#endif /* __linux__ */

static void print_uptime(uintmax_t now)
{
	const uintmax_t days = now / 86400;
	const uintmax_t hours = now / 3600;
	const uintmax_t minutes = now / 60;

	if (days > 0) {
		printf("up %" PRIuMAX "d %" PRIuMAX "h %" PRIuMAX "m\n",
			days, hours % 24, minutes % 60);
	} else if (hours > 0) {
		printf("up %" PRIuMAX "h %" PRIuMAX "m\n",
			hours % 24, minutes % 60);
	} else {
		printf("up %" PRIuMAX "m\n",
			minutes % 60);
	}
}

int main(int argc, char *argv[])
{
	uintmax_t now = 1;
	struct timespec ts = { 0L, 0L };

	UNUSED(argc);

	argv0 = argv[0];
	if (get_uptime(&ts, &now)) {
		return 1;
	}

	print_uptime(now);
	return 0;
}
