#if 0
COMPILE start settimeofday gettimeofday ewrites exit localtime_r \
			  asctime_r strlen mktime die #ultodec  

#DEBUG			  

if [ -n "$DEBUG" ]; then
	COMPILE printf ultodec ltodec
	mini_buf 512
	STRIPFLAG " "
else
	SHRINKELF
	OPTFLAG -Os
#LDSCRIPT onlytext
fi

return
#endif

#define BUFLEN 256

void usage(){
	ewrites("Usage: setdate [-h] [-s seconds] [[year/]month/day] [Hour:min[:seconds]]\n"
			  "show/set the date with the formats: xxxx/xx/xx and xx:xx:xx\n"
			  "Alternatively:    -s [seconds since 1970]\n"
			  "the date has to be submitted within apostrophs.\n");
	exit(0);
}

int tonum(char *s){
	int ret = 0;
	while ( isdigit( *s ) ){
		ret = ret*10 + *s-'0'; 
		s++;
	}
	dbgf("tonum: %d",ret);
	return(ret);
}


ml_size_t multodec(char *buf, unsigned long l){
	char *p = buf;
	ulong d = 1;

	while ( (d*10 < l) && (d*10<ULONG_MAX) )
		d*=10;

	while ( d ){
		uint i = l/d;
		l-=d*i;
		*p++ = '0' + i;
		d/=10;
	}
	return(p-buf);
}


int main( int argc, char **argv ){

	struct tm t;
	struct timeval tv;
	//struct timezone tz;
	int ret,l=0,seconds=0;
	char buf[256];
	char *fmt = "%Y/%m/%d %H:%M:%S%n";

	gettimeofday(&tv,0);

	if ( argc>1 && argv[1][0] == '-' && argv[1][1] == 's' ){
		seconds = 1;
		if ( argc > 2 ){
			tv.tv_sec = tonum( argv[2] );
			l = -11; // set date
		}
	} else {
		localtime_r( &tv.tv_sec, &t );
		for ( char *arg; *argv && (arg=*++argv); ){
			if ( ( l=strlen( *argv )) < 5 )
				usage();
			if ( l>9 && ( arg[4] == '/' || arg[4] == '-' ) ){
				t.tm_year = tonum( argv[0] ) - 1900;
				arg+=5;l-=5;
			}
			if ( l>4 ){
				if ( arg[2]=='/' || arg[2] == '-' ){  // month/day
					t.tm_mon = tonum( arg ) - 1;
					t.tm_mday = tonum( arg + 3 );
					if ( arg[5]== ' ' && l>10 ){ // sep space to hour
						l-=6;
						arg = &arg[6];
					}
				}
				if ( arg[2] == ':' ){ //hour:minute
					t.tm_hour = tonum( arg );
					t.tm_min = tonum( &arg[3] );
					if ( arg[5]==':' )
						t.tm_sec = tonum( &arg[6] );
				}
			}
		}
	}

	if ( l ){
		if ( l != -11 ) // got seconds (-s)
			tv.tv_sec = mktime( &t );
		if ( (ret=settimeofday( &tv, 0 ))<0 )
#ifndef DEBUG
			die(ret,"Couldn't set the system clock");
#else
			dbgf("err: %d",-ret);
#endif
	}



#if 0
	while ( *++argv ){
		if ( **argv == '-' ){
			switch ( argv[0][1] ){

				case 's':
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


					break;
				default:	
					usage();
			}
		} else if ( **argv == '+' ){
			fmt = &argv[0][1];
		} 
	}
#endif
	
	int len = 0;
	if ( seconds ){
		//len = ultodec(buf,tv.tv_sec,0,0,0);
		len = multodec(buf,tv.tv_sec);
		buf[len++] = '\n';
	} else {
		if ( fmt ){
			localtime_r( &tv.tv_sec, &t );
			len = strftime( buf, BUFLEN, fmt, &t );
			if ( len<1 )
				usage();
		}
	}

	write( 1, buf, len );
	//dbgf("mktime: %ld\n",mktime( &t ));

	exit(0);
}

