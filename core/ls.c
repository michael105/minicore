#if 0
COMPILE strcpy ewrites fchdir ml_getcwd
mini_start
mini_lstat
mini_chdir
mini_sprintf

mini_malloc_brk
mini_memcpy
mini_realloc
mini_getdents
mini_prints
mini_eprintf
mini_writes
mini_open
mini_close
mini_qsort
mini_alphasort
#mini_errno

mini_mmap
mini_strcmp

mini_eprintfs

mini_buf 8000
mini_printf
mini_itodec
mini_ltodec
mini_free_brk
mini_brk
mini_setbrk

mini_ansicolors

mini_syscalls

COMPILE ultohex strftime prints printsl localtime_r time growablemem getcwd strcat

globalregister
OPTFLAG -Os
PICFLAG "-fpic -pie"

STRIPFLAG ' '
#LDSCRIPT 
#shrinkelf
#FULLDEBUG

return
#endif
// ls. first version.
// todo: 
// extract the options (macros for big capitals)
// rewrite listdir ( scan and stat at once )
// 	fstatat and openat
// 	(don't change directories)
// rewrite argument scanning
// 


/*
enum shortopts { 
	opt_A,opt_B,opt_C,opt_D,opt_E,opt_F,opt_G,opt_H,opt_I,opt_J,opt_K,opt_L,opt_M,
	opt_N,opt_O,opt_P,opt_Q,opt_R,opt_S,opt_T,opt_U,opt_V,opt_W,opt_X,opt_Y,opt_Z,
	opt_a,opt_b,opt_c,opt_d,opt_e,opt_f,opt_g,opt_h,opt_i,opt_j,opt_k,opt_l,opt_m,
	opt_n,opt_o,opt_p,opt_q,opt_r,opt_s,opt_t,opt_u,opt_v,opt_w,opt_x,opt_y,opt_z
};*/

enum shortopts { 
	opt_A=01,opt_B=02,opt_C=04,opt_D=010,opt_E=020,opt_F=040,opt_G=0100,opt_H=0200,
	opt_I=0400,opt_J=01000,opt_K=02000,opt_L=04000,opt_M=010000,opt_N=020000,
	opt_O=040000,opt_P=0100000,opt_Q=0200000,opt_R=0400000,opt_S=01000000,
	opt_T=02000000,opt_U=04000000,opt_V=010000000,opt_W=020000000,opt_X=040000000,
	opt_Y=0100000000,opt_Z=0200000000,opt_a=0400000000,opt_b=01000000000,
	opt_c=02000000000,opt_d=04000000000,opt_e=010000000000,opt_f=020000000000,
	opt_g=040000000000,opt_h=0100000000000,opt_i=0200000000000,opt_j=0400000000000,
	opt_k=01000000000000,opt_l=02000000000000,opt_m=04000000000000,
	opt_n=010000000000000,opt_o=020000000000000,opt_p=040000000000000,
	opt_q=0100000000000000,opt_r=0200000000000000,opt_s=0400000000000000,
	opt_t=01000000000000000,opt_u=02000000000000000,opt_v=04000000000000000,
	opt_w=010000000000000000,opt_x=020000000000000000,opt_y=040000000000000000,
	opt_z=0100000000000000000 };

// better let the praeprocessor do the work, than at runtime..
#define _LIT_A 'A'
#define _LIT_B 'B'
#define _LIT_C 'C'
#define _LIT_D 'D'
#define _LIT_E 'E'
#define _LIT_F 'F'
#define _LIT_G 'G'
#define _LIT_H 'H'
#define _LIT_I 'I'
#define _LIT_J 'J'
#define _LIT_K 'K'
#define _LIT_L 'L'
#define _LIT_M 'M'
#define _LIT_N 'N'
#define _LIT_O 'O'
#define _LIT_P 'P'
#define _LIT_Q 'Q'
#define _LIT_R 'R'
#define _LIT_S 'S'
#define _LIT_T 'T'
#define _LIT_U 'U'
#define _LIT_V 'V'
#define _LIT_W 'W'
#define _LIT_X 'X'
#define _LIT_Y 'Y'
#define _LIT_Z 'Z'
#define _LIT_a 'a'
#define _LIT_b 'b'
#define _LIT_c 'c'
#define _LIT_d 'd'
#define _LIT_e 'e'
#define _LIT_f 'f'
#define _LIT_g 'g'
#define _LIT_h 'h'
#define _LIT_i 'i'
#define _LIT_j 'j'
#define _LIT_k 'k'
#define _LIT_l 'l'
#define _LIT_m 'm'
#define _LIT_n 'n'
#define _LIT_o 'o'
#define _LIT_p 'p'
#define _LIT_q 'q'
#define _LIT_r 'r'
#define _LIT_s 's'
#define _LIT_t 't'
#define _LIT_u 'u'
#define _LIT_v 'v'
#define _LIT_w 'w'
#define _LIT_x 'x'
#define _LIT_y 'y'
#define _LIT_z 'z'

//  QUOTE(x) gets 'x'
#define QUOTE(a) _LIT_##a

#define OPT_A 01
#define OPT_B 02
#define OPT_C 04
#define OPT_D 010
#define OPT_E 020
#define OPT_F 040
#define OPT_G 0100
#define OPT_H 0200
#define OPT_I 0400
#define OPT_J 01000
#define OPT_K 02000
#define OPT_L 04000
#define OPT_M 010000
#define OPT_N 020000
#define OPT_O 040000
#define OPT_P 0100000
#define OPT_Q 0200000
#define OPT_R 0400000
#define OPT_S 01000000
#define OPT_T 02000000
#define OPT_U 04000000
#define OPT_V 010000000
#define OPT_W 020000000
#define OPT_X 040000000
#define OPT_Y 0100000000
#define OPT_Z 0200000000
#define OPT_a 0400000000
#define OPT_b 01000000000
#define OPT_c 02000000000
#define OPT_d 04000000000
#define OPT_e 010000000000
#define OPT_f 020000000000
#define OPT_g 040000000000
#define OPT_h 0100000000000
#define OPT_i 0200000000000
#define OPT_j 0400000000000
#define OPT_k 01000000000000
#define OPT_l 02000000000000
#define OPT_m 04000000000000
#define OPT_n 010000000000000
#define OPT_o 020000000000000
#define OPT_p 040000000000000
#define OPT_q 0100000000000000
#define OPT_r 0200000000000000
#define OPT_s 0400000000000000
#define OPT_t 01000000000000000
#define OPT_u 02000000000000000
#define OPT_v 04000000000000000
#define OPT_w 010000000000000000
#define OPT_x 020000000000000000
#define OPT_y 040000000000000000
#define OPT_z 0100000000000000000

void setopt( char c, long *opts ){
	if ( c>= 'A' && c <= 'X' )
		*opts |= (long)1<<( c-'A');
	else if ( c>='a' && c<='z' )
		*opts |= ((long)1<<( c-'a'+26));
}

int opt( char c, long opts ){
	if ( c>= 'A' && c <= 'X' )
		return(opts & (long)1<<( c-'A') ? 1 : 0 );
	return( opts & ((long)1<<( c-'a'+26)) ? 1 : 0 );
}


void usage(){
		ewrites("Usage: ls [-h]  \n");
		exit(1);
}

int de_match(const struct dirent *de){
		//if ( match(de->d_name,"*.c",0) == 0 )
		//		return(1);
		return(1);
}

typedef struct listcolor{
	int mode;
	char c;
	//char align;
	char* color;
} _listcolor;

struct listcolor colors[] = {
	{S_IFLNK,'l',AC_LCYAN},
	{S_IFDIR,'d',AC_BLUE},
	{0111,'-'   ,AC_LGREEN}, // executable
	{S_IFREG,'-',""},
	{S_IFIFO,'p',AC_BROWN},
	{S_IFBLK,'b',AC_YELLOW},
	{S_IFCHR,'c',AC_YELLOW},
	{S_IFSOCK,'s',AC_YELLOW},
	{0}
};

//"globals"
DEFINE_GLOBALS( { 
		struct tm tmnow; 
		long opts;
		struct stat* pstat;
		struct stat* statmem;
		} );

#define amemset(s,c,n) { asm("push %%rdi\nrep stosb\npop %%rdi": :"a"(c), "c"(n), "D"(s) : "memory" ); };
static inline void* memset(void* s, int c, int n) { 
	//char *sp = s;
	//asm("rep stosb": "=D"(sp) : "D"(sp), "a"(c), "c"(n) : "memory" ); 
	//asm("push %%rdi\nrep stosb\npop %%rdi": :"a"(c), "c"(n), "D"(s) : "memory" ); 
#if 0
	asm( R"(
	push %%rdi 
	mov %%al,%%ah
	mov %%ax,%%dx
	shl $16,%%eax
	or %%eax,%%edx
	mov %%edx,%%eax
	shl $32,%%rax
	or %%rdx,%%rax
	mov %%rcx,%%rdx
	mov %%rdi, %%rbx
	shr $3,%%rcx
	rep stosq
	mov %%edx,%%ecx
	and $0x3f, %%ecx
	rep stosb 
	pop %%rdi
)": :"a"(c), "c"(n), "D"(s) : "rdx", "rbx", "r12", "memory" ); 
#else
#if 0
	asm( R"(
	push %%rdi 
	mov %%al,%%ah
	jz _mzero
	push %%ax
	push %%ax
	push %%ax
	push %%ax
	pop %%rax
_mzero:
	mov %%rcx,%%rdx
	mov %%rdi, %%rbx
	shr $3,%%rcx
	rep stosq
	mov %%edx,%%ecx
	and $0x3f, %%ecx
	rep stosb 
	pop %%rdi
)": :"a"(c), "c"(n), "D"(s) : "rdx", "rbx", "r12", "memory" ); 
#else
#if 1
	asm( R"(
	push %%rdi 
	mov %%rcx,%%rbx
	shr $3,%%rcx
jz _issmaller
	mov $0x101010101010101,%%rdx
	mul %%rdx
	rep stosq
	add %%rbx,%%rdi
	and $0x3f,%%rbx
	sub %%rbx,%%rdi
_issmaller:
	mov %%rbx,%%rcx
	rep stosb 
	pop %%rdi
)": :"a"(c), "c"(n), "D"(s) : "rdx", "rbx", "r12", "memory" ); 
#if 0
	asm( R"(
	push %%rdi 
	mov %%rcx,%%rbx
	shr $6,%%rcx
jz _issmaller
	imul $0x1010101,%%eax,%%edx
jz _isnull
	and $0xff,%%rbx
	mov %%rdx,%%rax
	shl $32,%%rdx
	or %%rdx,%%rax
_isnull:
	rep stosq
_issmaller:
	mov %%rbx,%%rcx
	rep stosb 
	pop %%rdi
)": :"a"(c), "c"(n), "D"(s) : "rdx", "rbx", "r12", "memory" ); 
#endif

#else
	asm( R"(
	push %%rdi 
	mov %%al,%%ah
	movbe %%eax,%%edx
	or %%eax,%%edx
	movbe %%rdx,%%rax
	or %%rdx,%%rax
	mov %%rcx,%%rdx
	and $0x3f, %%edx
	shr $3,%%rcx
	mov %%rdi, %%rbx
	rep stosq
	mov %%edx,%%ecx
	rep stosb 
	pop %%rdi
)": :"a"(c), "c"(n), "D"(s) : "rdx", "rbx", "r12", "memory" ); 
#endif
#endif
#endif
	return(s);
};

void * __attribute__((noinline))xmemset( void *s, int c, int n){
#if 1
	asm( R"(
	mov %esi,%eax
	mov %edx,%ecx
	rep stosb 
	mov %rdi,%rax
	retq
)");
	__builtin_unreachable();
#else
		int a;
		char *sp = s;
		for ( a=0; a<n; a++)
				sp[a] = (char)c;
		return(s);
#endif
}

char* humanize( char *buf, int dec, long l ){
	if ( l<=0 ){
		//*(long*)(buf) = 0x45452020202020;
		memset( buf, ' ', 6 );
		buf[7] = 0;
		
		if ( l==0 ){
			buf[6] = '0';
		} else {
			buf[5] = 'E';
			buf[6] = 'E';
		}
	} else if ( l<(1024)){
 		sprintf(buf,"%7u",l);
		return(buf);
	} else {
	char *p = "kMGTPE"; // E equals 1<<60. (Exa)
	long s = 1024*1024, os = 10, rs=10;
	while ( s < l ){
		os+=10;
		rs<<=10;
		s<<=10;
		p++;
	}
	long r = l-((l>>os)<<os);
	r=r/rs;
	sprintf(buf,"%3u.%02u%c",l>>os,r,*p);
	}

	return(buf);
}

// print a direntry; this assumes dirent.d_ino to be
// a pointer to a stat buf (The inode nr doesn't help so much anyways..)
// the pointer isn't used, when the according options (-l,-S,.. aren't set)
int printentry( char *path, struct stat* st, long opts ){
	char *perms = "rwx";
	char *pp = perms;
	char permstring[12];
	permstring[10]=0;
	int b = 1;

	//return(0);
	for ( int perm = 0400; perm; perm >>= 1 ){
		if ( st->st_mode & perm )
			permstring[b] = *pp;
		else 
			permstring[b] = '-';
		b++;pp++;
		if ( *pp==0 ) pp = perms;
	}

	if ( st->st_mode & S_ISUID )
		permstring[3] = 'S';
	if ( st->st_mode & S_ISGID )
		permstring[6] = 'S';
	if ( st->st_mode & S_ISVTX )
		permstring[9] = 'T';


	char *color="";

	for ( _listcolor *lc = colors; lc->mode!=0; lc++ ){
		if ( lc->mode == ( (st->st_mode) & lc->mode ) ){
			color = lc->color; permstring[0]=lc->c;
			break;
		}
	}

	//if ( st->st_mode & 0111 ) // executable
	//	color=AC_LGREEN;
	//prints(permstring);
	char sbuf[16];
humanize(sbuf,2, st->st_size); 

	printf("%s %4d %4d %s " , permstring,st->st_uid, st->st_gid,sbuf);
	struct tm tm;
	char buf[16];
	//static struct tm tmnow = { .tm_year=0 };
	if ( !(GLOBALS->tmnow.tm_year) ){
		time_t tnow;
		time((uint*)&tnow);
		localtime_r( &tnow, &GLOBALS->tmnow );
	}
	localtime_r( &st->st_mtime.tv_sec, &tm );
	char *fmt = "%b %e %H:%M ";
	if ( GLOBALS->tmnow.tm_year - tm.tm_year ){ // year differs
		fmt = "%b %e  %Y ";
	}
	strftime( buf, 16, fmt, &tm );

	printsl(buf, color,path,AC_NORM);

	return(0);
}


int invert;
int alphasort_r(const void *a,const void *b){
	return(-alphasort((const struct dirent**)a,(const struct dirent**)b));
}

int sizesort(const void *a,const void *b){
	return ( invert^(((struct stat*)(*(const struct dirent**)a)->d_ino)->st_size <
	((struct stat*)(*(const struct dirent**)b)->d_ino)->st_size) );
}

int accesstimesort(const void *a,const void *b){
	return ( invert^((ulong)(((struct stat*)(*(const struct dirent**)a)->d_ino)->st_mtime.tv_sec) <
	(ulong)(((struct stat*)(*(const struct dirent**)b)->d_ino)->st_mtime.tv_sec)) );
}


typedef struct _direntry{
	struct dirent* dent;
	struct stat* st;
} direntry;


int listdir(const char* dir,long opts){
	// save heap start
	long addr = getbrk();
	struct dirent **list;

#define _BUFSIZE 4096

	int fd;
	if ((fd = open(dir, O_RDONLY|O_DIRECTORY|O_CLOEXEC)) < 0){
		//seterrno(fd);
		return(fd);
	}
	char *buf;
	buf= malloc_brk(_BUFSIZE);
	if ( buf==0 ){
		//seterrno(ENOMEM);
		return(-ENOMEM);
	}
	int len;
	int pos = 0;
	int cnt = 0;
	int cp = 0;
	int oldcp = 0;
	int bufpos=0;
	int oldpos=0;
	while ( (len = getdents( fd, (struct dirent*) (buf+bufpos), _BUFSIZE) )>0){
		while ( pos < len + bufpos ){
			struct dirent *de = (void *)(buf+pos);
			pos+=de->d_reclen;
			// select here.
			//if ( !(fp_select && !(fp_select(de))) ){ // selected
				cnt++;
				cp += de->d_reclen;
				//printf("%s\n", de->d_name );
			//}
			if ( oldcp < oldpos ){
				//copy
				memcpy(buf+oldcp,buf+oldpos,de->d_reclen);
			}
			oldcp=cp;
			oldpos=pos;
		}
		bufpos=pos;
		buf=realloc(buf,bufpos+_BUFSIZE);
		//printf("buf: %l, pos: %d, cp: %d\n",buf,pos,cp);
		if ( !buf) {
			//seterrno(ENOMEM);
			close(fd);
			return(-ENOMEM);
			break;
		}
	}

	if ( len<0 ){
		close(fd);
		//seterrno(len);
		return(len);
	}

	// alloc place for the pointer list, when needed
	if ( cnt*sizeof(POINTER) > _BUFSIZE+(pos-cp) ){
		//prints("realloc\n");
		realloc(buf,cp+(cnt*sizeof(POINTER)));
		if ( !buf ){
			//seterrno(ENOMEM);
			return(-ENOMEM);
		}
	}

	struct dirent *de;
	de = (void*)buf;
	list= (void*)(buf+cp);
	for(int a=0;a<cnt;a++){
		*list = de;
		*list++;
		de=(void*)de+de->d_reclen;
	}
	list = (void*)(buf+cp);

	if ( !(GLOBALS->pstat) ){
		GLOBALS->pstat=(GLOBALS->statmem=growablemem(cnt*sizeof(struct stat)));
	}

	// save the pointer, so the memory can be "free'd" on return
	struct stat* fpstat = GLOBALS->pstat;
	GLOBALS->pstat--;

	if ( chdir(dir) )
		eprintf("Error changing directory to %s, errno: %d\n",dir,errno);
	errno=0;

	for ( int a=0; a<cnt; a++ ){
		if ( lstat( list[a]->d_name,
						(struct stat*)GLOBALS->pstat) ){
			eprintf("Err, %s errno %d\n",list[a]->d_name,errno);// pstat is a pointer to the currently free stat buf
			errno=0;
		} else {
			list[a]->d_ino = (ino_t)GLOBALS->pstat;
			GLOBALS->pstat--;
		}
	}

	invert=0;
	int (*cmp)(const void*,const void*)= 
		(int(*)(const void*,const void*))alphasort;
	if ( opts & OPT_r ){
		cmp=alphasort_r;
		invert=1;
	}
	if ( opts & OPT_S ){
		cmp=sizesort;
	}
	if ( opts & OPT_t ){
		cmp=accesstimesort;
	}

	qsort( list, cnt, sizeof(POINTER), cmp );
	
	typedef struct { char *path; struct stat* st; } t_stat;
	t_stat* statlist = (t_stat*)GLOBALS->pstat;
	statlist --;
	t_stat* pstatl = statlist; 
		
	for ( int a=0; a<cnt; a++ ){
		printentry( list[a]->d_name, (struct stat*)list[a]->d_ino, opts );
		
		if ( (opts&OPT_R) && ( (((struct stat*)list[a]->d_ino)->st_mode) & S_IFDIR ) && 
				!((((struct stat*)list[a]->d_ino)->st_mode) & S_IFLNK  ) ){
			if ( !((list[a]->d_name[0] == '.' && list[a]->d_name[1] == 0 ) || 
				(list[a]->d_name[1] != 0 && list[a]->d_name[0] == '.' && list[a]->d_name[1] == '.' && list[a]->d_name[2] == 0 )) ){
				pstatl->st = (struct stat*)list[a]->d_ino;
				pstatl->path = list[a]->d_name;
				pstatl--; // grow used mem downwards
			}
		}
	}
	GLOBALS->pstat = (struct stat*)pstatl;
	char cwd[PATH_MAX];
	int l = ml_getcwd(cwd,PATH_MAX);
	
	while( pstatl != statlist ){
		pstatl++;
		printsl( "\n" AC_GREEN, cwd, "/", pstatl->path, ":" AC_NORM );
		//printsl( "\n" AC_GREEN, cwd, "/", (*pstatl)->de->d_name, ":" AC_NORM );
		listdir( pstatl->path, opts );
		//listdir( (*pstatl)->de->d_name, opts );
		fchdir(fd);
	}
	
	// free heap
	setbrk(addr);	
	// "free" mmap ( but don't unmap, possibly we need it later,
	// and there's no real reason to free mapped pages in a short running process
	GLOBALS->pstat = fpstat;
	close(fd);
	return(0);
}

int main(int argc, char **argv){
	IMPLEMENT_GLOBALS(globals);
	GLOBALS->opts = 0;
	GLOBALS->pstat = 0;
	struct stat st;

	char buf[16];
	printf("hu: -%s-\n",humanize(buf,2,-1));
	printf("hu: -%s-\n",humanize(buf,2,-1000));
	printf("hu: -%s-\n",humanize(buf,2,0));
	printf("hu: -%s-\n",humanize(buf,2,1090));
	printf("hu: -%s-\n",humanize(buf,2,100));

	char b2[140];
	char *b = memset(b2,'a',140);
	memset(b2+12,'b',14);
	memset(b2+1,'c',3);
	printf("b2: %s\n",b);
	memset(b2+60,'v',65);
	printf("b2: %s\n",b);


	char b3[1200];
	memset(b3,'x',800);
	printf("b3: %s\n",b3);

	return(0);

#define opts GLOBALS->opts
	// define to something other for parsing docu
#ifndef OPT
#define OPT(flag,desc,code) case QUOTE(flag): opts |= OPT_##flag; code; break;
#endif

	for ( *argv++; *argv && argv[0][0]=='-'; *argv++ ){
		for ( char *c = argv[0]+1; *c != 0; c++ ){
			switch ( *c ){
				OPT(h,"Show usage",usage());
				OPT(l,"Show extended info",);
				OPT(r,"reverse order",);
				OPT(S,"Sort by size",);
				OPT(t,"Sort by modification time",);
				OPT(R,"list directories recursive",);
				default:
					usage();
			} 
		} 
	}

	if ( *argv == 0 ){
		listdir(".",opts);
	} else {
		for (;*argv; *argv++){
			lstat( *argv, &st );
			if ( S_ISDIR( st.st_mode ) ){
				prints("\n",*argv,":\n");
				listdir(*argv,opts);
			} else {
				printentry( *argv, &st, opts );
			}
		}
	}

	return(0);
}
