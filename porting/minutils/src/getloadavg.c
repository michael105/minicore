#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#define UNUSED(x)	((void)(x))

int main(int argc, char *argv[])
{
	double up[3];

	UNUSED(argc);
	UNUSED(argv);

	if (getloadavg(up, ARRAY_SIZE(up)) < 0) {
		fprintf(stderr, "%s: getloadavg() failed\n", argv[0]);
		return 1;
	}

	printf("%.2f %.2f %.2f\n", up[0], up[1], up[2]);
	return 0;
}
