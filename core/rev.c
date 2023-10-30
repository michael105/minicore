#if 0

COMPILE write read nread sys_brk open
#COMPILE printf fmtp fmtu fmtd

SHRINKELF
SEGMENTS text 

return
#endif




MAIN{

#	define BUFS PAGESIZE

	// replace with sbrk. should be also usable in
	// combination with malloc
	char* brkcur;  
	char* brkstart = brkcur = (char*)sys_brk(0);
	char* brkend = (char*)sys_brk((ulong)brkstart+BUFS);

void rev(int fd){
	char buf[BUFS];
	int l;

	while ( (l = nread( fd, (char*)brkcur, brkend-brkcur)) > 0 ){
		brkcur += l;
		if ( brkend - BUFS < brkcur ){
			brkend = (char*)sys_brk( (ulong)brkend+BUFS );
		}
	}
	//printf("len: %u\n---\n",len);
	char *pos = brkcur -1;
	while ( pos > brkstart + BUFS ){
		for ( char *b=buf, *p=pos; b<buf+BUFS; b++,p-- )
			*b=*p;
		write(1,buf,BUFS);
	}
	for ( char *b=buf, *p=pos; p>=brkstart; b++,p-- )
		*b=*p;
	write(1,buf,pos-brkstart+1);

	brkcur = brkstart;
	if ( brkend > brkstart + 4*BUFS )
		brkend = (char*)sys_brk( (ulong)brkstart + BUFS );
}

	*argv++;

	if ( *argv == 0 ){
		rev(0);
	} else {
		for ( char *f = *argv; *argv; *argv++ ){
			int fd = open(f,O_RDONLY);
			if ( fd>0 ){
				rev(fd);
				close(fd);
			}
		}
	}

	exit(0);
}



