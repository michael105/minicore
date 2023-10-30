#if 0

COMPILE write read nread sys_brk open
#COMPILE printf fmtp fmtu fmtd

SHRINKELF
SEGMENTS text 
#STRIPFLAG -s

return
#endif




MAIN{

# define BUFS PAGESIZE

	char* brkcur;  
	char* brkstart = brkcur = (char*)sys_brk(0);
	char* brkend = (char*)sys_brk((ulong)brkstart+BUFS);
	//printf("%p\n%p\n",brkstart,brkend);

 void tac(int fd){
	int l;
	while ( (l = nread( fd, (char*)brkcur, brkend-brkcur)) > 0 ){
		brkcur += l;
		if ( brkend - BUFS < brkcur ){
			brkend = (char*)sys_brk( (ulong)brkend+BUFS );
		}
	}

	char *pos = brkcur-1;
	for( char *p = brkcur-2; p>=brkstart; p-- ){
		if ( *p == '\n' ){
			write(1,p+1,pos-p);
			pos=p;
		}
	}
	if ( pos > brkstart )
		write(1,brkstart,pos-brkstart+1);

	brkcur = brkstart;
	if ( brkend > brkstart + 4*BUFS ) // release memory
		brkend = (char*)sys_brk( (ulong)brkstart + BUFS );
 }

	//printf("%p\n%p\n",brkstart,brkend);

	*argv++;

	if ( *argv == 0 ){
		tac(0);
	} else {
		for ( char *f = *argv; *argv; *argv++ ){
			int fd = open(f,O_RDONLY);
			if ( fd>0 ){
				tac(fd);
				close(fd);
			}
		}
	}

	exit(0);
}



