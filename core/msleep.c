#ifdef mlconfig

COMPILE nanosleep eprintsl 

#SEGMENTS text
SHRINKELF

return
#endif

// misc 2023/10
// BSD license

void usage(){
		writes("Usage: msleep val[.fraction][s|m|h|d] [val[.fraction][s|m|h|d]]\n");
		exit(1);
}

ulong tolong( const char c[] ){
	ulong r = 0;
	union {
		uint ret[2];
		ulong l;
	} u; 

	do {
		u.l=0;
		int dot = 0;
		uint frac = 1;

		while ( *c > 47 && *c < 58 ){
			u.ret[dot] = u.ret[dot] * 10 + (*c-'0');
			c++;		

			if ( dot )
				frac *= 10;

			if ( *c == '.' ){
				dot = 1;
				c++;
			}
		}

		switch (*c) {
			case 'w': u.l *= 7;
			case 'd': u.l *= 24;
			case 'h': u.l *= 60;
			case 'm': u.l *= 60;
			case ' ':
			case '\0':
			case 's': u.l *= 1000;
			case 'l': // milliseconds
		 break;
		 default: usage();
		}
		c++;
		r += u.ret[0];
		r += u.ret[1] / frac;
	} while ( *c > 47 && *c < 58 );

	return(r); // time in milliseconds
}

int main(int argc, char *argv[]){

	//ewritesl("ok");
	//eprintsl(argv[1]);
	if ( (argc < 2) || argv[1][0] == '-' ) {
			usage();
	}
	
	//ewritesl("ok");
	ulong v = 0;
	for ( *argv++; *argv; *argv++ )
		v+= tolong( *argv );
	//ewritesl("ok");
	//printf("ret: %lu\n",v );	
	struct timespec ts;
	ts.tv_sec = v/1000;
	ts.tv_nsec = (v%1000)*1000000;
	
	int r;
	while ( (r=nanosleep(&ts,&ts)) <0 && (r==-EINTR) );

	return 0;
}
