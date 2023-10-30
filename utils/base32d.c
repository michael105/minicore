#if 0
COMPILE memcpy errno
SHRINKELF

return
#endif




// the syscalls slow down.
// would be possible to spare several writes,
// by inserting the linebreaks into the outputbuffer,
// and enlarge the buffer.
// slows down from 150MB/s to 30 MB/s
// but. hey.

// BUF has to be a multiple of 5
#define BUF 2000

// Byteswap
#define BSWAP(a) asm("bswap %0" : "+r"(a) )


// read with repetition on eintr,..,  needed especially for reading pipes,
// if the buffer runs empty
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



int main(int argc, char **argv){
	char buf[BUF];
	char obuf[BUF];

	int r;
	int len = 0;
	unsigned long l;
	int marge = 64; // marge, needed for weeding out linebreaks, etc.
	char *bs = buf;
	int err = 0;

	while( marge ) {
		r=nread(0,bs,BUF-marge); 
		if ( r<BUF-marge )
			*(long*)((char*)bs+r) = (long)0;
		if ( r == 0 ){ // end of input
			marge=0;
		}
		char *pbuf = buf;
		char *pobuf = obuf;
		int lb = 0;

		while ( pbuf + marge < bs+r ){
			l = 0;

			for ( int a = 0; a<8; a ++ ){
				while ( *pbuf && ( (*pbuf < '2') || (*pbuf >'Z') ) ){ // weed out other chars
					pbuf++;
					lb++;
					if ( pbuf > (bs+r) ){
						write(1,"Bad input\n",10);
						err = EINVAL;
						continue;
					}
				}
				char c = *pbuf;

				if ( !(c == '=' || c==0) ){ // not at the end
					//c -= 65;
					//c += (c>>8)&41; // is a number, add 41 ( or 0 )
					if ( c >= 'A' )
						c-='A';
					else
						c -= 24;

					l = ( l << 5 ) | c;
				} else { 
					// end of input / ==
					l <<= 5;
					marge = 0;
					if ( ! len )
						len = (( pbuf - buf -lb  ) * 5) >> 3;
				}
				pbuf ++;
			}

			BSWAP(l);
			l >>= 24;
			*(long*)pobuf = l;
			pobuf += 5;
		}

		// write
		if ( marge ){ 
			write(1,obuf,(pobuf-obuf));
			memcpy(buf,pbuf,(bs+r)-pbuf);
			bs = (buf+((bs+r)-pbuf));
		} else {
			write(1,obuf,len );
		}

	};


	exit(err);
}

