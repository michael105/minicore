#if 0
mini_start
mini_write
#mini_strlen
LDSCRIPT textandbss
INCLUDESRC
shrinkelf
return
#endif


int main(int argc, char *argv[], char *envp[]){

	if ( argc == 1 ){
		for (;;){ 
			write(1,"y\n",2);
		}
	} else {
		for ( int a=2; a<argc; a++ ){
			argv[a][-1] = ' ';
		};
		//int len = strlen(argv[argc-1])+argv[argc-1]-argv[1];
		int len = *envp-argv[1];
		argv[1][len] = '\n';
		for ( ;; ){
			write( 1,argv[1],len+1 );
		}
	}

	return(0);
}

