### Static builds for linux amd64

Compiled and linked to minilib with the standard toolchain, gcc and ld,  -Os;

I'm trying to get a usable basic Unix system (without kernel) built statically within 64kB,
it's going to be interesting.

(Admittedly, possibly it's going to be 128kB. But not more.)

The commands here all shall dump there usage on invocation with -h, showing the implemented arguments.
(Coding this now)

Just now I'm thinking about adding a tool "help" - which could be invoked by the other tools,
also to dump the usage. 

Some sources in this directory are originally written by ammongit (github.com/ammongit/minutils) 
and modified by me.

I also reused some fragments of minix.
I kept all copyright notices within the sources.


```
basename        2019-08-19             493
cat             2019-08-20             1038
chroot          2020-06-04             567
echo            2020-01-19             368
false           2019-08-19             153
getenv          2019-08-19             782
head            2019-08-20             1407
ls              2020-01-15             13344
mkdir           2020-06-08             208
pivot_root      2020-06-04             271
pwd             2020-01-19             305
rm              2020-06-08             500
rmdir           2020-06-08             504
tail            2019-08-20             2576
tee             2019-08-19             2335
touch           2020-06-04             464
true            2019-08-19             150
uname           2020-01-19             1362
yes             2020-01-15             309
===============================================
                          size: 27136 Bytes
```
