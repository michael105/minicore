#if 0
globals_on_stack
mini_errno
mini_start
mini_printsl
mini_prints
mini_writes
mini_getenv
mini_environ
mini_where_l
mini_INCLUDESRC
LDSCRIPT onlytext
SHRINKELF
OPTFLAG -Os
#STRIPFLAG ' '
return
#endif


int main( int argc, char *argv[], char *envp[] ){
	int r = 1;

	if ( argc < 2 ){
		writes("usage: where executable executable ..");
	} else {
		char buf[PATH_MAX];

		for ( *argv++; *argv; *argv++ ){
			const char *iter = 0;
			r=1;
			while ( where_l(*argv,&iter,buf)  ){
				printsl(buf);
				r = 0;
			} 		
			if ( r ) {
				prints( *argv, " not found\n" );
			}

		}
	}

	exit(r);
}

