#if 0
 COMPILE start printf dprintf prints printsl eprintfs itooct itohex stat writes \
	 ewrites exit_errno itodec ltodec ultohex localtime_r


 mini_buf 512
 INCLUDESRC
 shrinkelf
return
#endif


void usage(){
	writes("usage: stat [-h] path1 [path2...]\n");
	exit(0);
}

void printtime( const char* s, const struct timespec *tms ){
	struct tm t;
	localtime_r(&tms->tv_sec,&t);
	printf("%s: %d-%02d-%02d %02d:%02d:%02d.%09ld\n", s,
			t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, tms->tv_nsec );
}

int main(int argc, char *argv[]){

	if ( argc>1 && argv[1][0] == '-' && argv[1][1] == 'h' ) 
		usage();

	struct stat st;
	int e=0;
	int tmp;
	char *perms = "rwx";
	char *pp = perms;
	char permstring[12];
	permstring[11]=0;



	typedef struct {
		int t;
		char *s;
		char c;
	} _types;

#define F(x,str,chr) {.t=S_IF##x,.s=str,.c=chr}

	_types types[]= { 
		F(IFO,"FIFO",'p'),
		F(CHR,"Char",'c'),
		F(DIR,"Directory",'d'),
		F(BLK,"Block",'b'),
		F(REG,"File",'-'),
		F(LNK,"Link",'l'),
		F(SOCK,"Socket",'s'),
		{.t=0,.s="",'-'}
		};

	for ( *argv++; *argv; *argv++ ){
		if ( (tmp=stat(*argv, &st) ) ){
			eprintfs("Couldn't access %s\n",*argv);
			e=tmp;
		} else {
			printsl("File: ",*argv);
			printf("Size: %ld\t\tBlocks: %ld\t\tIO Block: %ld\t\t",
					st.st_size,st.st_blocks,st.st_blksize);

			_types *t = types; 
			while( t->t && !(t->t&st.st_mode) )
				t++;
			prints( t->s );
			permstring[0]=t->c;

			printf("\nDevice: %lxh/%dd\tInode: %ld\t\tLinks: %d\n",
					st.st_dev,st.st_dev,st.st_ino,st.st_nlink);

			char *ps = permstring+1;
			for ( int perm = 0400; perm; perm >>= 1 ){
				if ( st.st_mode & perm )
					*ps = *pp;
				else 
					*ps = '-';
				ps++;pp++;
				if ( *pp==0 ) pp = perms;
			}
			if ( st.st_mode & S_ISUID )
				permstring[3] = 'S';
			if ( st.st_mode & S_ISGID )
				permstring[6] = 'S';

			printf("Access: 0%o/%s\tUid: %d\t\tGid: %d\n",
					st.st_mode&07777,permstring,st.st_uid,st.st_gid);
			printtime("Access",&st.st_atime);
			printtime("Modify",&st.st_mtime);
			printtime("Change",&st.st_ctime);

		}
	}
	if ( e )
		exit_errno(e);
	exit(0);
}
