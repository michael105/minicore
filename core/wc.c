

#define wc_OPTIONS enum { h,c,l,m,w }
#define OPT(func,opts,option) ({ func##_OPTIONS; opts & (1<<option); })
#define SETOPT(func,opts,option) ({ func##_OPTIONS; opts = (opts | ( 1 << option )); })

#define TOOL wc
#define _S(x) #x

#define HELP( tool, str ) void help_##TOOL(){ writes( str ); exit(0); }
// or execute help id
#define CALL_HELP help_##TOOL()


HELP( wc, "Usage: "" [-gGnruhx] [user]" R"(


