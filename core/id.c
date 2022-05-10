#if 0
COMPILE start eprintsl getuid printsl writes writesl ewrites uitodec getgrnam \
 getgrgid printfs eprintfs prints printf itodec
COMPILE getpwuid pwent getusergroups getpwnam


INCLUDESRC
SHRINKELF
#STRIPFLAG " "
LDSCRIPT onlytext
globals_on_stack
MINIBUF 512
#FULLDEBUG

return
#endif

#define id_OPTIONS enum { g, G, n,r,u,h }
#define OPT(func,opts,option) ({ func##_OPTIONS; opts & (1<<option); })
#define SETOPT(func,opts,option) ({ func##_OPTIONS; opts = (opts | ( 1 << option )); })



void usage(){
	writesl("id [user]");
	exit(0);
}

void printud(uint id){
	char buf[16];
	uitodec( buf, id, 0,0,0 );
	prints(buf);
	//exit(0);
}

int id_main( FILE* out, FILE* err, int opts, const char* user[] ){
	



	return(0);
}

int main(int argc, char *argv[]){

	const char* options = "gGnruh";
	int opts = 0;

	for ( *argv++; *argv && argv[0][0]=='-'; *argv++ ){
		for ( char *opt = *argv+1; *opt; opt++ ){
			//printfs( "arg: %c\n", *opt );
			int a = 1;
			for ( const char *c = options; *c != *opt; *c++ ){
				if ( !*c ){
					eprintfs( "Unknown option: %c\n", *opt );
					exit(1);
				}
				a <<=  1;
			}
			opts |= a;
		}
	}

	if ( OPT(id,opts,h) )
		usage();

	struct passwd* pw;

	if ( !*argv ){
		int uid;
		if ( OPT(id,opts,r) ){
			writes("real\n");
			uid = getuid();
		} else {
			uid = geteuid();
		}

		pw = getpwuid( uid );
	} else {
		pw = getpwnam( *argv );
	}
	
	if ( !pw ){
		ewrites("Unable to read user entry\n");
		exit(1);
	}
	int _groups[128];
	int *groups = _groups;


	int ng = getusergroups( pw->pw_name, 128, groups );
	if ( ng == -1 ){
		ewrites("\nUnable to read group file\n");
		exit(2);
	}
	if ( OPT(id,opts,g) ){
		printf("%d\n",pw->pw_gid);
		exit(0);
	}
	if ( OPT(id,opts,u) ){
		printf("%d\n",pw->pw_uid);
		exit(0);
	}




	struct group *gr = getgrgid( pw->pw_gid );
	const char* fmt = "%s%d(%s)";
	const char* pref = " groups=";
	const char* preflist= ",";

	if ( OPT(id,opts,n) ){
		fmt="%!%!%s ";
	}

	
	if ( OPT(id,opts,G) ){
		fmt=" %!%d";

		printf("%d", pw->pw_uid);
	} else {
		printf(fmt, "uid=", pw->pw_uid,  pw->pw_name );
		printf(fmt,	"gid=", pw->pw_gid, gr ? gr->gr_name : "?" );
				//pw->pw_gid, gr ? gr->gr_name : "?" );
	}

	while ( ng-->0 ){
		gr = getgrgid( *groups );
		//if ( gr && ( gr->gr_gid != pw->pw_gid )){
		printf( fmt, pref, gr ? gr->gr_gid : 0, gr ? gr->gr_name : "?" );
		pref=preflist;
		//}
		groups++; 
	}
	writes("\n");

	exit(0);
}

