#if 0
mini_buf 4096
mini_start
mini_fprintfs
mini_itodec
mini_atoi
mini_itoHEX
mini_printf
mini_fprintf
mini_prints
mini_writes
mini_putchar
mini_strcmp
mini_strerror
mini_readdir
mini_opendir
mini_closedir
mini_fputs
mini_puts
mini_putchar
mini_malloc
mini_chdir
mini_errno
COMPILE stat
INCLUDESRC
HEADERGUARDS
shrinkelf
return
#endif

// source from minutils

#ifndef MLIB
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#endif



static struct {
	const char *argv0;
	bool all : 1;
	int opts;
} opt;

//TODO: this is worth a separate header file within minilib.
// It's simple and has low footprint.
enum options { a, l, h };
#define SET_OPT(args,opt) { args = ( args | (0x1 << opt) ); }
#define GET_OPT(args,opt)  ( args & (0x1 << opt) )
//#define SET_OPT(args,opt) { args = ( args | OPTBIT_##opt ) }
//#define OPTBIT_a 0x1

#define OPT(a) (1<<(a-96)) 
#define OPTIONS(opt) ( (1<<('a'-96)) | 


//int print_entry( struct dirent *ent, int args ){
int print_entry( char *name, int args ){
			if ( GET_OPT( args, l ) ){
					struct stat st;
					stat( name, &st );
					switch( st.st_mode & S_IFMT ){
							case S_IFREG: putchar('-');
														break;
							case S_IFDIR: putchar('d');
														break;
							default: putchar('-');
					}
					int p = 0;
					char c[]="rwx";
					for ( int i = 00400; i>0; i = i>> 1 ){
							if ( st.st_mode & i ) 
									putchar ( c[p] );
							else
									putchar( '-' );
							p++;
							if (p>2) p=0;
					}
					fprintf(stdout, "   %d %d ", st.st_uid, st.st_gid);
					fprintf(stdout, "%-9d    ", st.st_size);

					printf("%s\n", name);

			} else {
					printf("%s\n", name);
			}


			return(0);
}

static int list(const char *path, int label, int args)
{
	static int notfirst;
	struct stat statbuf;
	struct dirent *ent;
	DIR *dh;

	if (stat(path, &statbuf)) {
		fprintfs(stderr, "ls: unable to stat %s: %s\n",
			path, strerror(errno));
		return 1;
	}
	if (!S_ISDIR(statbuf.st_mode)) {
		//printf("%s\n", path);

		print_entry( path, args );
		return 0;
	}
	if (label) {
		if (notfirst)
			putchar('\n');
		notfirst = 1;
		printf("%s:\n", path);
	}

	dh = opendir(path);
	if (!dh) {
		fprintf(stderr, "ls: Error openening %s\n%d\n",
		 path, errno);
		return 1;
	}
	chdir(path);

	while ((ent = readdir(dh))) {
		if (ent->d_name[0] == '.' && ! GET_OPT(args,a))
			continue;
		print_entry( ent->d_name, args );
	}
	if (closedir(dh)) {
		fprintfs(stderr, "ls: %s: %s\n",
		  path, strerror(errno));
		return 1;
	}
	return 0;
}

/* Usage: ls [-a] [PATH...] */
int main(int argc, const char *argv[])
{
	int i;

	int args = 0;



	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			break;
		} 
		switch (argv[i][1]) {
			case 'a':
				opt.all = 1;
				SET_OPT(args,a);
				break;
			case 'l':
				SET_OPT(args,l);
				break;
			case 'h':
				writes( "ls [-a] [-l] [-h] [file(s) / dir(s)]\n" );
				return(0);
		default:
			fprintfs(stderr, "ls: invalid argument: %s\n", argv[i]);
			return(1);
		}
	}
	//printf("opt: %d\n", args );

	if (i == argc) {
		if (list(".", 0, args))
			return 1;
	} else if (i == argc - 1) {
		if (list(argv[i], 0, args))
			return 1;
	} else for (; i < argc; i++) {
		if (list(argv[i], 1, args))
			return 1;
	}
	return(0);
}
