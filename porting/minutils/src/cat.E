# 1 "cat.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "./minilib.mconf.h" 1
# 9 "./minilib.mconf.h"
# 1 "/home/micha/prog/minilib/minilib.h" 1
# 28 "/home/micha/prog/minilib/minilib.h"
# 1 "/home/micha/prog/minilib/minilib/include/headerguards.h" 1
# 29 "/home/micha/prog/minilib/minilib.h" 2





# 1 "/home/micha/prog/minilib/minilib/include/globaldefs.h" 1
# 43 "/home/micha/prog/minilib/minilib/include/globaldefs.h"
typedef long time_t;
typedef long suseconds_t;
typedef unsigned useconds_t;
struct timeval { time_t tv_sec; suseconds_t tv_usec; };






typedef int FILE;







typedef union { FILE *F; int i, fd;} mfl_union;
# 35 "/home/micha/prog/minilib/minilib.h" 2
# 1 "/home/micha/prog/minilib/minilib/include/syscall.h" 1
# 26 "/home/micha/prog/minilib/minilib/include/syscall.h"
# 1 "/home/micha/prog/minilib/headers/linux_x64/sys/syscall.h" 1
# 27 "/home/micha/prog/minilib/minilib/include/syscall.h" 2
# 36 "/home/micha/prog/minilib/minilib.h" 2
# 1 "/home/micha/prog/minilib/minilib/include/syscall_stubs.h" 1
# 15 "/home/micha/prog/minilib/minilib/include/syscall_stubs.h"
# 1 "/home/micha/prog/minilib/minilib/include/timeval.h" 1


# 1 "/home/micha/prog/minilib/minilib/include/time_t.h" 1


typedef long time_t;
# 4 "/home/micha/prog/minilib/minilib/include/timeval.h" 2
# 14 "/home/micha/prog/minilib/minilib/include/timeval.h"
struct timezone {
  int tz_minuteswest;
  int tz_dsttime;
};
# 16 "/home/micha/prog/minilib/minilib/include/syscall_stubs.h" 2




extern int sysret;
extern int errno;

struct stat;


























# 61 "/home/micha/prog/minilib/minilib/include/syscall_stubs.h"
inline int volatile __attribute__((always_inline)) rename( const char* a1, const char* a2 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 82 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }


inline int volatile __attribute__((always_inline)) getpid( ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 39 | 0 ) | 0 ) ) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }


inline int volatile __attribute__((always_inline)) lseek( unsigned int a1, int a2, int a3 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 8 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) , "d" (a3) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }


inline int volatile __attribute__((always_inline)) time( unsigned int *a1 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 201 | 0 ) | 0 ) ) , "D" (a1) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }


inline int volatile __attribute__((always_inline)) ftruncate( unsigned int a1, unsigned int a2 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 77 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }


inline int volatile __attribute__((always_inline)) write( int a1, const void *a2, int a3 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 1 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) , "d" (a3) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }


inline int volatile __attribute__((always_inline)) fstat( int a1, struct stat* a2 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 5 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }


inline int volatile __attribute__((always_inline)) gettimeofday( struct timeval *a1, struct timezone *a2 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 96 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }


inline int volatile __attribute__((always_inline)) dup2( int a1, int a2 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 33 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }


inline int volatile __attribute__((always_inline)) dup3( int a1, int a2, int a3 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 292 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) , "d" (a3) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }


inline int volatile __attribute__((always_inline)) unlink( const char* a1 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 87 | 0 ) | 0 ) ) , "D" (a1) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }


inline int volatile __attribute__((always_inline)) getcwd( char *a1, long a2 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 79 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }


inline int volatile __attribute__((always_inline)) stat( const char* a1, struct stat* a2 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 4 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }


inline int volatile __attribute__((always_inline)) mprotect( unsigned long int *a1, unsigned long int a2, int a3 ){ asm volatile ("syscall" : "=a" (sysret) : "a" ( (10 | 0 ) ) , "D" (a1) , "S" (a2) , "d" (a3) : "memory","rcx", "r11" ); if ( sysret<0 ){ errno = -sysret; return(-1);} return(*a1); }


inline int volatile __attribute__((always_inline)) fsync( int a1 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 74 | 0 ) | 0 ) ) , "D" (a1) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }


inline int volatile __attribute__((always_inline)) dup( int a1 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 32 | 0 ) | 0 ) ) , "D" (a1) : "memory","rcx", "r11" ); return( (sysret<0) ? -1 : sysret ); }
# 37 "/home/micha/prog/minilib/minilib.h" 2
# 1 "/home/micha/prog/minilib/minilib/include/timeval.h" 1
# 38 "/home/micha/prog/minilib/minilib.h" 2
# 1 "/home/micha/prog/minilib/minilib/headers/common/sys/types.h" 1
# 13 "/home/micha/prog/minilib/minilib/headers/common/sys/types.h"
# 1 "/home/micha/prog/minilib/headers/common/bits/alltypes.h" 1
# 25 "/home/micha/prog/minilib/headers/common/bits/alltypes.h"
typedef unsigned char uchar;






typedef int wchar_t;

typedef long double double_t;

typedef float float_t;

typedef struct { long long __ll; long double __ld; } max_align_t;





typedef struct { union { int __i[14]; volatile int __vi[14]; unsigned long __s[7]; } __u; } pthread_attr_t;

typedef struct { union { int __i[10]; volatile int __vi[10]; volatile void *volatile __p[5]; } __u; } pthread_mutex_t;

typedef struct { union { int __i[10]; volatile int __vi[10]; volatile void *volatile __p[5]; } __u; } mtx_t;

typedef struct { union { int __i[12]; volatile int __vi[12]; void *__p[6]; } __u; } pthread_cond_t;

typedef struct { union { int __i[12]; volatile int __vi[12]; void *__p[6]; } __u; } cnd_t;

typedef struct { union { int __i[14]; volatile int __vi[14]; void *__p[7]; } __u; } pthread_rwlock_t;

typedef struct { union { int __i[8]; volatile int __vi[8]; void *__p[4]; } __u; } pthread_barrier_t;




typedef unsigned long uintptr_t;

typedef long ptrdiff_t;

typedef long ssize_t;

typedef long intptr_t;

typedef long regoff_t;

typedef long register_t;

typedef signed char int8_t;

typedef signed short int16_t;

typedef signed int int32_t;

typedef signed long int64_t;

typedef signed long intmax_t;

typedef unsigned char uchar;

typedef unsigned short uint16_t;

typedef unsigned int uint;

typedef unsigned long uint64_t;

typedef unsigned long u_int64_t;

typedef unsigned long uintmax_t;

typedef unsigned mode_t;

typedef unsigned long nlink_t;

typedef long off_t;

typedef unsigned long ino_t;

typedef unsigned long dev_t;

typedef long blksize_t;

typedef long blkcnt_t;

typedef unsigned long fsblkcnt_t;

typedef unsigned long fsfilcnt_t;

typedef unsigned wint_t;

typedef unsigned long wctype_t;

typedef void * timer_t;

typedef int clockid_t;

typedef long clock_t;



struct timespec { time_t tv_sec; long tv_nsec; };

typedef int pid_t;

typedef unsigned id_t;

typedef unsigned uid_t;

typedef unsigned gid_t;

typedef int key_t;





typedef struct __mbstate_t { unsigned __opaque1, __opaque2; } mbstate_t;

typedef struct __locale_struct * locale_t;

typedef struct __sigset_t { unsigned long __bits[128/sizeof(long)]; } sigset_t;

struct iovec { void *iov_base; long iov_len; };

typedef unsigned socklen_t;

typedef unsigned short sa_family_t;
# 14 "/home/micha/prog/minilib/minilib/headers/common/sys/types.h" 2

typedef unsigned char u_int8_t;
typedef unsigned short u_int16_t;
typedef unsigned u_int32_t;
typedef char *caddr_t;
typedef unsigned char u_char;
typedef unsigned short u_short, ushort;
typedef unsigned u_int, uint;
typedef unsigned long u_long, ulong;
typedef long long quad_t;
typedef unsigned long long u_quad_t;
# 1 "/home/micha/prog/minilib/headers/common/endian.h" 1
# 26 "/home/micha/prog/minilib/minilib/headers/common/sys/types.h" 2
# 39 "/home/micha/prog/minilib/minilib.h" 2




# 1 "/home/micha/prog/minilib/minilib/include//utils.h" 1
# 44 "/home/micha/prog/minilib/minilib.h" 2

# 1 "/home/micha/prog/minilib/minilib/macros/vararg.h" 1
# 46 "/home/micha/prog/minilib/minilib.h" 2


# 1 "/home/micha/prog/minilib/minilib/include/stdarg.h" 1
# 9 "/home/micha/prog/minilib/minilib/include/stdarg.h"
typedef __builtin_va_list va_list;
# 49 "/home/micha/prog/minilib/minilib.h" 2



# 1 "/home/micha/prog/minilib/minilib/include/exit.h" 1
# 53 "/home/micha/prog/minilib/minilib.h" 2
# 1 "/home/micha/prog/minilib/minilib/include/minilib_global.h" 1
# 16 "/home/micha/prog/minilib/minilib/include/minilib_global.h"
typedef struct {
  int align;
  int srand;
  int pstream;
  int mbufsize;
  int stream[16];
  union {
    int ibuf[1024>>2];
    char mbuf[1024];
  };
} minilib_globals;

extern minilib_globals ml;
# 54 "/home/micha/prog/minilib/minilib.h" 2


struct udiv_t { unsigned int quot, rem; };
# 77 "/home/micha/prog/minilib/minilib.h"
# 1 "/home/micha/prog/minilib/compat/errno.h" 1





# 1 "/home/micha/prog/minilib/include/errno.h" 1
# 136 "/home/micha/prog/minilib/include/errno.h"
extern int errno;
# 7 "/home/micha/prog/minilib/compat/errno.h" 2


# 1 "/home/micha/prog/minilib/src/errno.c" 1





int sysret;
int errno;
# 10 "/home/micha/prog/minilib/compat/errno.h" 2
# 78 "/home/micha/prog/minilib/minilib.h" 2
# 289 "/home/micha/prog/minilib/minilib.h"
int dprintf( int fd, const char *fmt, ... );
# 309 "/home/micha/prog/minilib/minilib.h"
# 1 "/home/micha/prog/minilib/minilib/include/close.h" 1





static inline int volatile __attribute__((always_inline)) close( int fd ){
  int ret;
  asm volatile ("syscall" : "=a" (ret) : "a" ( (3 | 0 ) ) , "D" ((int)fd) : "memory","rcx", "r11" );
  return(ret);
}
# 310 "/home/micha/prog/minilib/minilib.h" 2
# 324 "/home/micha/prog/minilib/minilib.h"
# 1 "/home/micha/prog/minilib/include/syscall.h" 1
# 325 "/home/micha/prog/minilib/minilib.h" 2
# 1 "/home/micha/prog/minilib/minilib/include/read.h" 1







# 1 "/home/micha/prog/minilib/macros/overrun.h" 1






# 1 "/home/micha/prog/minilib/macros/../include/exit.h" 1
# 8 "/home/micha/prog/minilib/macros/overrun.h" 2
# 1 "/home/micha/prog/minilib/macros/../include/mprints.h" 1


extern int _mprints(char*msg,...);
# 9 "/home/micha/prog/minilib/macros/overrun.h" 2
# 9 "/home/micha/prog/minilib/minilib/include/read.h" 2



static inline int volatile __attribute__((always_inline)) read( int fd, void* buf, int len ){


  int ret;
  asm volatile ("syscall" : "=a" (ret) : "a" ( (0 | 0 ) ) , "D" (fd), "S" ((unsigned long int)buf), "d" (len) : "memory","rcx", "r11" );
  return(ret);
}
# 326 "/home/micha/prog/minilib/minilib.h" 2
# 345 "/home/micha/prog/minilib/minilib.h"
# 1 "/home/micha/prog/minilib/minilib/include/mini_stdio.h" 1



# 1 "/home/micha/prog/minilib/minilib/include/stdarg.h" 1
# 5 "/home/micha/prog/minilib/minilib/include/mini_stdio.h" 2
# 18 "/home/micha/prog/minilib/minilib/include/mini_stdio.h"
static int close(int);
static int read(int fd, void *buf, int len);


static inline int __attribute__((always_inline)) fileno( FILE *f ){
  return( *f & 0x3fffffff );
}







static inline int __attribute__((always_inline)) fclose( FILE* f ){
  int fd = *f;
  *f = -1;

  if ( f[1] == ml.stream[ml.pstream] )
    for ( ml.pstream--; ml.stream[ml.pstream-1] == -1; ml.pstream-- );

  return( close(fd) );
}
# 68 "/home/micha/prog/minilib/minilib/include/mini_stdio.h"
static inline long fwrite(const void *ptr, long size, long nmemb, FILE *f){
  const void *p = ptr;
  int a;
  for ( a = 0; a<nmemb; a++ ){
    if ( write( fileno(f), p, size ) != size ){
      *f = *f | 0x40000000;
      return(a);
    }
    p = p + size;
  }
  return(a);
}



static inline long ftell(FILE *f){
   return(lseek( fileno(f), 0, 1 ));
}



static inline void fgetpos(FILE *f, long *pos ){
   *pos = ftell(f);
}



static inline int fsetpos(FILE *f, int pos ){
   int r = lseek( fileno(f), pos, 0 );
  if ( r==pos ){
    *f = *f & 0x3fffffff;
    return(r);
  }
  return(r);
}





static inline int fseek(FILE *f, long offset, int whence ){
   int r = lseek( fileno(f), offset, whence );
  if ( r>0 )
    *f = *f & 0x3fffffff;
  return(r);
}




static inline void rewind( FILE *f ){
  fseek(f, 0, 0);
}



static inline long fread(void *ptr, long size, long nmemb, FILE *f){
  void *p = ptr;
  int a;
  for ( a = 0; a<nmemb; a++ ){
    if ( read( fileno(f), p, size ) != size ){
      *f = *f | 0x80000000 ;
      return(a);
    }
    p = p + size;
  }
  return(a);
}
# 146 "/home/micha/prog/minilib/minilib/include/mini_stdio.h"
static inline int feof(FILE *f){
  if ( *f & 0xc0000000 )
    return(1);
  return(0);
}
# 346 "/home/micha/prog/minilib/minilib.h" 2
# 365 "/home/micha/prog/minilib/minilib.h"
int snprintf( char *buf, long size, const char *fmt, ... );
# 380 "/home/micha/prog/minilib/minilib.h"
int volatile open( const char *s, int flags, ... );
# 663 "/home/micha/prog/minilib/minilib.h"
# 1 "/home/micha/prog/minilib/minilib.c" 1
# 43 "/home/micha/prog/minilib/minilib.c"
# 1 "/home/micha/prog/minilib/minilib.h" 1
# 44 "/home/micha/prog/minilib/minilib.c" 2
# 1 "/home/micha/prog/minilib/minilib/asm/start.c" 1
# 13 "/home/micha/prog/minilib/minilib/asm/start.c"
# 1 "/home/micha/prog/minilib/minilib/asm/start-linux-x64.c" 1

void _start(){
# 1 "/home/micha/prog/minilib/src/startup.c" 1






ml.mbufsize = 1024;
ml.stream[0]=0;
ml.stream[1]=1;
ml.stream[2]=2;
ml.pstream = 3;
# 4 "/home/micha/prog/minilib/minilib/asm/start-linux-x64.c" 2
__asm__("#.global _start\n#_start:\n	popq %rdi\n	movq %rsp,%rsi\n	leaq  8(%rsi,%rdi,8),%rdx\n"
# 14 "/home/micha/prog/minilib/minilib/asm/start-linux-x64.c"
 "call main\n	movq %rax, %rdi\n.global _exit\n_exit:\n	movq $60, %rax\n	syscall\n"





 );
};
# 14 "/home/micha/prog/minilib/minilib/asm/start.c" 2
# 45 "/home/micha/prog/minilib/minilib.c" 2
# 1 "/home/micha/prog/minilib/minilib/src/minilib_global.c" 1
# 11 "/home/micha/prog/minilib/minilib/src/minilib_global.c"
minilib_globals ml;
# 46 "/home/micha/prog/minilib/minilib.c" 2


# 1 "/home/micha/prog/minilib/minilib/src/mprints.c" 1



# 1 "/home/micha/prog/minilib/minilib/src/../include/mprints.h" 1
# 5 "/home/micha/prog/minilib/minilib/src/mprints.c" 2
# 1 "/home/micha/prog/minilib/minilib/src/../include/stdarg.h" 1
# 6 "/home/micha/prog/minilib/minilib/src/mprints.c" 2
# 46 "/home/micha/prog/minilib/minilib/src/mprints.c"
int _mprints(char *msg,...){
  va_list args;
  __builtin_va_start(args,msg);
  int a;
  int ret = 0;

  do {
    a = 0;
    while (msg[a] != 0 ){
      a++;
    }
    ret += write(1,msg,a);
    msg = __builtin_va_arg(args,char*);
  } while ( msg != 0 );
  __builtin_va_end(args);
  return(ret);
}
# 49 "/home/micha/prog/minilib/minilib.c" 2



int errno;
int sysret;
# 70 "/home/micha/prog/minilib/minilib.c"
# 1 "/home/micha/prog/minilib/minilib/src/msprintf.c" 1
# 13 "/home/micha/prog/minilib/minilib/src/msprintf.c"
# 1 "/home/micha/prog/minilib/minilib/src/../macros/overrun.h" 1
# 14 "/home/micha/prog/minilib/minilib/src/msprintf.c" 2

# 1 "/home/micha/prog/minilib/minilib/src/../include/exit.h" 1
# 16 "/home/micha/prog/minilib/minilib/src/msprintf.c" 2
# 28 "/home/micha/prog/minilib/minilib/src/msprintf.c"
int vsnprintf(char *buf, long size, const char* fmt, va_list args ){



  int a = 0;
  int b = 0;
  int overflow = 0;
  int end, padding, c,groups=0;
  char sep;
  char tmp[16];
  char *s;


  while ( fmt[a] != 0){
    if ( fmt[a] == '%' ){
      end=0;
      padding = 0;
      sep = 0;
      groups=0;
      do {
        a++;
        if ( (fmt[a] > 47 ) && (fmt[a] < 58 ) ){
          c = 0;
          do {
            tmp[c] = fmt[a];
            a++;
            c++;
          } while ( (fmt[a] > 47 ) && (fmt[a] < 58 ) );
          tmp[c] = 0;



        }
        switch (fmt[a]){
          case '%':
            buf[b] = '%';
            end=1;
            b++;
            if ( b>= size ){ overflow=1; goto msprintfout;};
            break;

          case 'u':




            end=1;
            break;
          case 'd':




            end=1;
            break;

          case 'f':






            end=1;
            break;
          case 'l':




            end=1;
            break;

          case 39:
            sep = '.';
            break;
          case 'x':




            end=1;
            break;
          case 'X':




            end=1;
            break;

          case 'b':





            end=1;
            break;
          case '(':
            a++;
            if ( (fmt[a] > 47 ) && (fmt[a] < 58 ) ){
              c = 0;
              do {
                tmp[c] = fmt[a];
                a++;
                c++;
              } while ( (fmt[a] > 47 ) && (fmt[a] < 58 ) );
              a--;
              tmp[c] = 0;



            }
            break;
          case 's':
            s = __builtin_va_arg(args,char*);
            c=0;
            while(s[c] != 0){
              buf[b] = s[c];
              c++;
              b++;
              if ( b>= size ){ overflow=1; goto msprintfout;};
            }
            end=1;
            break;
          case 'c':
            buf[b] = __builtin_va_arg(args,int);
            b++;
            end=1;
            if ( b>= size ){ overflow=1; goto msprintfout;};
            break;






        }

      } while ((end==0) && (fmt[a+1] != 0 ));

    } else {
      buf[b] = fmt[a];
      b++;
      if ( b>= size ){ overflow=1; goto msprintfout;};
    }
    a++;
  }
msprintfout:
  buf[b] = 0;
  __builtin_va_end(args);



  if ( overflow )
    return(-b);
  else
    return(b);

}





int dprintf( int fd, const char *fmt, ... ){
  va_list args;
  __builtin_va_start(args,fmt);
  int r = vsnprintf(ml.mbuf, ml.mbufsize, fmt, args);
  if ( r<0 ){
    write(2,"Buffer Overrun.\n",16);
    asm("jmp _exit"::"D"(1));
  }
  write( fd, ml.mbuf, r );
  __builtin_va_end(args);
  return(r);
}





int snprintf( char *buf, long size, const char *fmt, ... ){
  va_list args;
  __builtin_va_start(args,fmt);
  int r = vsnprintf(buf, size, fmt, args);
  __builtin_va_end(args);
  return(r);
}
# 71 "/home/micha/prog/minilib/minilib.c" 2
# 80 "/home/micha/prog/minilib/minilib.c"
# 1 "/home/micha/prog/minilib/minilib/src/open.c" 1






# 1 "/home/micha/prog/minilib/include/filemodes.h" 1
# 8 "/home/micha/prog/minilib/minilib/src/open.c" 2

# 1 "/home/micha/prog/minilib/include/stdarg.h" 1
# 10 "/home/micha/prog/minilib/minilib/src/open.c" 2


int volatile open( const char *s, int flags, ... ){
  int ret;
  va_list args;
  __builtin_va_start(args,flags);
  int mode = __builtin_va_arg(args,int);
  __builtin_va_end(args);

  asm volatile ("syscall" : "=a" (ret) : "a" ( (2 | 0 ) ) , "D" ((unsigned long int)s), "S" (flags), "d" (mode) : "memory","rcx", "r11" );
  return(ret);
}




inline int volatile __attribute__((always_inline)) creat( const char *s, int mode ){
  return(open( s, 00000100|00000001|00001000, mode) );
}
# 81 "/home/micha/prog/minilib/minilib.c" 2
# 664 "/home/micha/prog/minilib/minilib.h" 2
# 9 "./minilib.mconf.h" 2
# 1 "<command-line>" 2
# 1 "cat.c"
# 1 "/home/micha/prog/minilib/compat/fcntl.h" 1
# 2 "cat.c" 2
# 1 "/home/micha/prog/minilib/compat/unistd.h" 1
# 3 "cat.c" 2


# 1 "/home/micha/prog/minilib/compat/stdio.h" 1
# 6 "cat.c" 2
# 1 "/home/micha/prog/minilib/compat/string.h" 1
# 7 "cat.c" 2

static const char *argv0;

static int cat(int fd)
{
 char buf[4096];
 ssize_t n;

 while ((n = read(fd, buf, sizeof(buf))) > 0) {
  if (write(1, buf, n) != n) {
   fprintf(&ml.stream[2], "%s: %s\n",
    argv0, strerror(errno));
   return 1;
  }
 }
 return 0;
}


int main(int argc, const char *argv[])
{
 int i;

 argv0 = argv[0];
 if (argc == 1) {
  return cat(0);
 } else for (i = 1; i < argc; i++) {
  int fd;

  fd = open(argv[i], 00000000);
  if (fd < 0) {
   fprintf(&ml.stream[2], "%s: %s: %s\n",
    argv[0], argv[i], strerror(errno));
   return 1;
  }
  if (cat(fd))
   return 1;
  if (close(fd)) {
   fprintf(&ml.stream[2], "%s: %s: %s\n",
    argv[0], argv[i], strerror(errno));
   return 1;
  }
 }
 return 0;
}
