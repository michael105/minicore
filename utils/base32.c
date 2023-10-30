#if 0
COMPILE errno
SHRINKELF
#STRIPFLAG
#OPTFLAG -O3

return
#endif

/* 
 base32 encoder
 encodes from stdin to stdout
 uses the (standard) encoding A-Z 2-8
 There's one optional argument, the position of the linebreak.
 e.g. base32 80
 default is 76
 0 means no linebreaks at all

 The branchless approach wasn't really faster. (git/base32)
 This uses the instructions bswap, ror and rol, 64 bit.
 (might be present at most systems)

 (c)2022, Michael (misc) Myer, GPL

*/

#ifndef MLIB
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#endif

// the syscalls slow down.
// would be possible to spare several writes,
// by inserting the linebreaks into the outputbuffer,
// and enlarge the buffer.
// slows down from 150MB/s to 30 MB/s
// but. hey.
// enlarging BUF and set wrap to 0 (disable)
// would also spare syscalls.
// (I'm a bit buffled, that the write syscalls are that expensive..)

// BUF has to be a multiple of 5
#define BUF 4000


// Byteswap
#define BSWAP(a) asm("bswap %0" : "+r"(a) )

// bitrotation 
#define ROL(bits,a) asm("rol $"#bits",%0" : "+r"(a) )
#define ROR(bits,a) asm("ror $"#bits",%0" : "+r"(a) )


// read with repetition on eintr,..,  needed especially for reading pipes,
// when the buffer runs empty
int nread( int fd, char *buf, int len ){
	char *b = buf;
	char *e = buf+len;

	do {
		errno = 0;
		int ret = read(fd,b,(e-b));
		if ( ret <= 0 ){
			if ( errno == EAGAIN || errno == ENOMEM || errno == EINTR )
				continue;
			return( b-buf ); // rw bytes (if)
		}
		b+=ret;
	} while ( b < e );

	return( len );
}

// rnd: 194 MB/s
// -> rol -> 198 MB/s
// -O3 208MB/s
char* base32enc(char *obuf, const char* buf, uint len){
	const	char *pbuf = buf;
	char *pobuf = obuf;

	while ( pbuf < buf+len ){
		long l = *(long*)pbuf;
		BSWAP(l);
			ROL(5,l);
		//for ( int a = 0; a<8; a++ ){
		//asm("mark:");
		for ( int a = 8; a-->0; ){
			char c = (l&0x1f);
			ROL(5,l);
			// convert to A-Z / 2-8 
			if ( c < 26 )
				c+=65;
			else
				c+=24;

			*pobuf = c;
			pobuf++;
		}
		pbuf += 5;
	}

	// fill end of a not fully filled buffer with ==
	if ( pbuf > buf + len ){
		//r*=8;
		len<<=3;
		int i = (pobuf-obuf-1) * 5;
		char *p = pobuf;
		while ( i > len ){
			*(--p) = '=';
			i-=5;
		}
	}

	return(pobuf);
}


//#define writes(s) write(1,s,sizeof(s));
void usage(){
	writes("usage: input | base32 [-d] [pos of linebreak]\n");

	exit(1);
}


int main(int argc, char **argv){
	char buf[BUF+8];
	char obuf[BUF*8/5+64];

	int r;
	int wrap = 76; // wrap
	int wrote = 0;

	if ( argc > 1 ){
		wrap = 0;
		for ( char *p = argv[1]; *p; p++ ){
			if ( *p < '0' || *p > '9' )
				usage();
			wrap = wrap*10 + *p-'0';
		}
	}

	while( (r=nread(0,buf,BUF)) > 0 ) {

		if ( r < BUF )
			*(long*)(buf+r) = (long)0;

		char *pobuf = base32enc(obuf,buf,r);
			
		// wrapping and output
		char *ob = obuf;
		if ( wrap ){
			while ( (pobuf-ob) >= wrap-wrote ){
				write(1,ob,wrap-wrote);
				write(1,"\n",1);
				ob += wrap-wrote;
				wrote = 0;
			}
		}
		wrote = write(1,ob,(pobuf-ob));
	};

	if ( wrote )
		write(1,"\n",1);

	exit(0);
}
