#if 0

COMPILE prints,strftime,time,exit,write,start,localtime_r,itodec,ewrites
SHRINKELF
LDSCRIPT textandbss
#DEFINE mini_longtimenames
return
#endif

#define BUFLEN 512

void usage(int ret){
	ewrites("Usage: date [-h] [+format] [-I[FMT]]\n");
	exit(ret);
}


int main(int argc, char**argv){
	char buf[BUFLEN];
	unsigned int t;
	struct tm ptm;

	time(&t);
	localtime_r((time_t*)&t,&ptm);

	char *fmt = "%a %b %e %T %Y%n";

	while ( *++argv ){
		if ( **argv == '-' ){
			switch ( argv[0][1] ){

				case 's':
					ewrites("Please use 'setdate' to set the clock.\n");
					exit(EINVAL);
#if 0
					if ( !*++argv || ( l=strlen( *argv )) < 5 )
						usage();
					if ( argv[0][2] == ':' ){
						t.tm_hour = tonum( *argv );
						t.tm_min = tonum( &argv[0][3] );
						if ( argv[0][5]==':' )
							t.tm_sec = tonum( &argv[0][6] );
					}
					tv.tv_sec = mktime( &t );
					if ( (ret=settimeofday( &tv, 0 ))<0 )
						//die(ret,"Couldn't set the time\n");
						printf("err: %d\n",-ret);
#endif
					break;
				case 'I':
					switch (argv[0][2]){
						case 's':
							fmt="%Y-%m-%dT%H:%M:%S%n";
							break;
						case 'm':
							fmt="%Y-%m-%dT%H:%M%n";
							break;
						case 'h':
							fmt="%Y-%m-%dT%H%n";
							break;

						default:
							fmt="%Y-%m-%d%n";
					}
					break;

				case 'h':
					usage(0);
				default:	
					usage(EINVAL);
			}
		} else if ( **argv == '+' ){
			fmt = &argv[0][1];
		} 
	}


	int len = strftime( buf, BUFLEN, fmt, &ptm );
	if ( len<1 )
		usage(EINVAL);
	write(1,buf,len);
	exit(0);
}
