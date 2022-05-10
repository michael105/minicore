
void usage(){
	writes("tee file1 file2 ... [-a file3 file4 ...]\n");
	exit(0);
}

int main(int argc, const char *argv[])
{
	char buf[4096];
	ssize_t n;
	int flags = O_TRUNC;
	int fds[MAXSPLIT];
	fds[0] = STDOUT_FILENO;
	int *fd = fds + 1;

	if ( argc == 1 || (( argv[1][0] == '-' ) && (argv[1][1] == 'h')) ){
		usage();
	}

	for ( *argv++; *argv; *argv++ ){
		if ( argv[0][0] == '-' ){
			switch ( argv[0][1] ) {
				default:
					eprintsl("Unknown option: ",argv[0]);
				case 'h':
					usage();
				case 'a' :
					flags = O_APPEND;
			}
		} else {
			if ((*fd = open(argv[0], O_CREAT | O_WRONLY | flags, 0666))<0){
				eprintsl("Couldn't open ", argv[0] );
				exit_errno(*fd);
			}
			fd++;
			if ( fd>=(fds+MAXSPLIT) ){
				eprintsl("Too many splits");
				exit(1);
			}
		}
	}
	*fd = 0;

	while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
		int *f = fds; 
		do {
			int e;
			if ((e=write( *f, buf, n)) != n){ // write with splice to stdout ?
				eprintsl(
					"Couldn't open ", argv[0]);
				exit_errno(e);
			}
			f++;
		} while ( *f );
	}

	for ( int *f = (fds+1); *f; f++ )
		close(*f);

	exit(0);
}
