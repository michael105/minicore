#if 0

COMPILE write getrandom writes PRINTF
SHRINKELF

#SEGMENTS text
#LDSCRIPT textonly
return
#endif

// get random from the linux kernel.
// by default: 8 Bytes, printed as hexadecimal and prefixed with 0x
		

#define SHORTOPTS_ENUM enum shortopts {                                          \
 opt_a=01,opt_b=02,opt_c=04,opt_d=010,opt_e=020,opt_f=040,opt_g=0100,opt_h=0200, \
 opt_i=0400,opt_j=01000,opt_k=02000,opt_l=04000,opt_m=010000,opt_n=020000,       \
 opt_o=040000,opt_p=0100000,opt_q=0200000,opt_r=0400000,opt_s=01000000,          \
 opt_t=02000000,opt_u=04000000,opt_v=010000000,opt_w=020000000,opt_x=040000000,  \
 opt_y=0100000000,opt_z=0200000000,opt_A=0400000000,opt_B=01000000000,           \
 opt_C=02000000000,opt_D=04000000000,opt_E=010000000000,opt_F=020000000000,      \
 opt_G=040000000000,opt_H=0100000000000,opt_I=0200000000000,opt_J=0400000000000, \
 opt_K=01000000000000,opt_L=02000000000000,opt_M=04000000000000,                 \
 opt_N=010000000000000,opt_O=020000000000000,opt_P=040000000000000,              \
 opt_Q=0100000000000000,opt_R=0200000000000000,opt_S=0400000000000000,           \
 opt_T=01000000000000000,opt_U=02000000000000000,opt_V=04000000000000000,        \
 opt_W=010000000000000000,opt_X=020000000000000000,opt_Y=040000000000000000,     \
 opt_Z=0100000000000000000 }

#define __SHORTOPTS_ENUM enum {                                      \
 a=01,b=02,c=04,d=010,e=020,f=040,g=0100,h=0200, \
 i=0400,j=01000,k=02000,l=04000,m=010000,n=020000,       \
 o=040000,p=0100000,q=0200000,r=0400000,s=01000000,          \
 t=02000000,u=04000000,v=010000000,w=020000000,x=040000000,  \
 y=0100000000,z=0200000000,A=0400000000,B=01000000000,           \
 C=02000000000,D=04000000000,E=010000000000,F=020000000000,      \
 G=040000000000,H=0100000000000,I=0200000000000,J=0400000000000, \
 K=01000000000000,L=02000000000000,M=04000000000000,                 \
 N=010000000000000,O=020000000000000,P=040000000000000,              \
 Q=0100000000000000,R=0200000000000000,S=0400000000000000,           \
 T=01000000000000000,U=02000000000000000,V=04000000000000000,        \
 W=010000000000000000,X=020000000000000000,Y=040000000000000000,     \
 Z=0100000000000000000 }



#define __USAGE(msg) void usage(){ \
	writes(msg); \
	exit(0); \
}

#define __DECL_OPT(opt,desc) opt
#define __USAGE_PARAM(opt,desc) 

#if 1 // help_inline
#define __usage_opt(opt,desc) " -" #opt " " desc "\n"


// define inline function to return all possible options, xored, ulong
#define DECLARE_OPTIONS(...) static inline ulong __short_options(){ \
	__SHORTOPTS_ENUM; \
	return ( FOREACH_PAIR(__DECL_OPT,|,__VA_ARGS__) ); \
}; \
static void __attribute__((noreturn)) usage(void){\
writes( " " USAGE "\n(-h: this help)\n");\
	writes( \
	FOREACH_PAIR(__usage_opt,,__VA_ARGS__)  \
		); \
exit(0);\
}


#endif

#define USAGE "[-xpdurkm] [-i value] [count]"

DECLARE_OPTIONS( x, "output hexadecimal (default)",
	         	  p, "prefix hexadecimal digits with 0x (default)",
	         	  d, "output decimal digits",
	         	  u, "read /dev/urand (nonblocking generator, theoretically insecure) ",
	         	  r, "     /dev/rand",
	         	  k, "use the kernel syscall (default)",
	         	  m, "use a marsaglia pseudo random generator, initiated with time and kernel random",
	         	  i, "'value' : initiate the marsaglia pseudo random generator with 'value'\n"
	         	     "          (given in decimal)");

//DECLARE_INT_OPTIONS(i);
#define random_INT_OPTIONS i

//DECLARE_ARGUMENTS(optional,int,8,"count in bytes");

//DEFAULT_OPTIONS(x,d, // default switch, switch(es) overwriting the default, ..,..
//	k,u|r|m|i);

#define __OPT_SHORT(x) x
#define OPTS(...) ({ __SHORTOPTS_ENUM; FOREACH( __OPT_SHORT,|,__VA_ARGS__); })

#define OPTS_ARG(...) 

typedef struct _opt_var{
	ulong opt;
	union {
		int i;
		ulong ul;
		long l;
		char* s;
		ulong value; // cast to pointer size
	};
} opt_var;

//typedef struct _arguments{
typedef union _arguments{
		ulong v;
		int i;
		ulong ul;
		long l;
		char *s;
	} arguments;
//} arguments;

#define VARCOUNT 7

#define _VAR(x,v) var[x].value = v;
#define TOOL(f,count,_args,opts,...) ({ __SHORTOPTS_ENUM; \
		_##f( count, ({ ulong args[] = { 1,2,3 } ; args; }), opts, ({ opt_var var[VARCOUNT]; \
					FOREACH_PAIR(_VAR,,__VA_ARGS__) var;}) ); \
				})

#define random(...) TOOL(random,__VA_ARGS__)

//#define ARGS(...) ({ arguments FOREACH(__ARG,,__ARG(a) ); })


//random(1,2,3)
#define _EXPAND2(x) x_main
#define _EXPAND(x) _EXPAND2(_##x)
#define TOOL_MAIN( toolname ) int _##toolname( int argc, ulong arg[], ulong opts, opt_var var[] )
// submit fd's - needed to redirect io to pipes / shared memory (or the globals in whole?)


TOOL_MAIN( random ){
	printf("argc: %d\n",argc);
	while( argc-->0 )
		printf("arg: %d = %d\n",argc,arg[argc]);

	printf("var 0 : %d\n",var[0].i );

	return(1);
}


MAIN{

//	opt_var args[2];

//	inline opt_var* random_args(opt_var *a,ulong opt,ulong value){
//		args[0].opt=opt; args[0].value=value ;
//		return args;
//	}	


	arguments argsx[] = { {.v=1},{.v=2} };


	//usage();
	int ret;
	if (0){
	//opt_var args[] = {{ .opt=1, .value=-13 }};
	//int ret = ({ opt_var args[] = {{ .opt=1, .value=-13 }};
	//random_main(8,OPTS(x), args ); });
//#define LIST(...) COUNT(__VA_ARGS__),CAT( __VA_ARGS__ )
#define LIST(...) 3,CAT( __VA_ARGS__ )
	random( LIST(1,2,3) ,x+r, i,13 ); 
	ulong aa[2];// = { 3,2 };
//{ _random(2, ({aa[0]=3;aa[1]=4;aa;}),OPTS(x), opt_var ax[] = ({ opt_var ax[3]; ax[0].i=11; ax[2].value=27; ax; }) ); }; // { };ax; }) );
	//random_main(8,OPTS(x), random_args(args,1,13) );
	//printf("l: %ld  i: %d\n",args->l,args->i);
}

	char buf[18];
	*(short*)buf = 0x7830; // = "0x"
	union {
		char inbuf[8];
		long l;
	} u;
	

	ret = getrandom( u.inbuf, 8, 0 );

	if ( ret<0 )
		exit(-ret);


	for ( char *b = buf +2; b<buf+18; b++){
		*b = (u.l & 0xf);
		*b = *b>9 ? *b+('a'-10) : *b + '0';
		u.l = u.l>>4;
	}
	write(1,buf,18);

	exit(0);
}
