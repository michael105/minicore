#if 0

COMPILE ewrites writes exit_errno syscalls setuid setgid strlen execvpe environ
DEFINE mini_no_hardening

#GLOBALS onstack

SHRINKELF
#STRIPFLAG
LDSCRIPT textandbss

#DEFINE mini_globals 0

#echo "char** environ;"
#FULLDEBUG
return
#endif

// sudo to drop privileges to nobody
// has to be installed with suid bit,
// and owner root.
// The intention is to be able for users 
// to drop privileges, and execute programs as "nobody".
//
// this depends on having an account nobody, 
// (uid and gid 66534, according to "standards")
// setup with according privileges.

#define uid_nobody 65534
#define gid_nobody 65534



void usage(){
		writes("usage: nbdo [-h] command [arguments]\n"
						"Execute 'command' with id and gid of nobody (65534)\n" );
		exit(1);
}

int main(int argc,	char **argv, char **envp ){
		environ = envp;

		gid_t group = gid_nobody;

		if (argc < 2 || (argv[1][0]=='-' && argv[1][1] == 'h' ) ){
				usage();
		}

		int ret;
		char *fail;

#define IFFAIL(a,str) if ( (ret=a) ){fail = str; goto failed;}

		IFFAIL(sys_setgroups(1,&group),"setgroups()");
		IFFAIL(setgid(gid_nobody),"setgid()");
		IFFAIL(setuid(uid_nobody),"setuid()");

		argv++;
		ret = execvpe( *argv, argv, envp );

		fail = *argv;
		ewrites("Execute ");
failed:
		write(STDERR_FILENO,fail,strlen(fail));
		ewrites(" failed\n");
		exit_errno(ret);
}
