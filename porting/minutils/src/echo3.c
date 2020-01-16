#if 0
mini_start
mini_putchar
INCLUDESRC
exit 0
#endif

#include "minilib.h"
/* Usage: echo [TEXT...] */
int main(int argc, char *argv[]){
		int n;
		if ( argc && argv[1][0] == '-' && argv[1][1] == 'n' ) 
				n=2;
		else 
				n=1;

		for ( int a = n; a<(argc);  a++  ){
				for ( char *c = argv[a]; *c!=0; c++ )
						putchar(*c);
				putchar(' ');
		}
		if ( n==1 )
				putchar( '\n' );
		return 0;
}
