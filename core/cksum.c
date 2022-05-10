#if 0
COMPILE printf itodec eprintf prints open eprints
MINIBUF 512
SHRINKELF
#STRIPFLAG " "

return
#endif

void crc32_init(uint32_t *crc_table)
{
	uint32_t c,i;
	unsigned j;

	*crc_table = 0;
	for (i = 0x1000000; i > 0xfffff; i+=0x1000000) { // 1 - 256 for little_endian
		crc_table++;
		c = i;// << 24;
		for (j = 8; j; j--) {
			asm("mark:\n");
			asm("add %0,%0\njnc 1f\nxor $0x04c11db7,%0\n1:\n" : "+r"(c) );
			//			c = c&0x80000000 ? ((c <<1 ) ^ 0x04c11db7) : ( c<<1 );
			// c = (c&1) ? ((c >> 1) ^ polynomial) : (c >> 1); // little
		}
		*crc_table = c;
	}
}

	
int cksum(int fd, const char *s, uint32_t* crctab){
	ssize_t n;
	size_t len = 0, i;
	uint32_t ck = 0;
	unsigned char buf[BUFSIZ];

	while ((n = read(fd, buf, BUFSIZ)) > 0){
		for (i = 0; i < n; i++)
			//ck = (ck >> 8) ^ crctab[(ck ^buf[i]) &0xff];
			ck = (ck << 8) ^ crctab[(ck >> 24) ^ buf[i]];
		len += n;
	}
	if (n < 0) {
		eprints("couldn't read ", s, "\n" );
		return(1);
	}

	for (i = len; i; i >>= 8)
	//	ck = (ck >> 8) ^ crctab[(ck ^i) &0xff];
		ck = (ck << 8) ^ crctab[(ck >> 24) ^ (i & 0xFF)];

	printf("%u %u %s\n", ~ck, len,s);
	return(0);
}

int main(int argc, char *argv[]){
	int fd;
	int ret = 0;

	uint32_t crctab[256];
	crc32_init(crctab);
	argv++;

	do{
		if ( *argv==0 || (argv[0][0]=='-' && argv[0][1]==0) ){
			ret |= cksum(0, "<stdin>", crctab);
		} else {
			if ((fd = open(*argv, O_RDONLY)) < 0) {
				eprints("Couldn't open ", *argv, "\n");
				ret = 1;
			} else {
				ret |= cksum(fd, *argv, crctab);
				close(fd);
			}
		}
	} while ( *argv && *(++argv) );


	exit(ret);
}

