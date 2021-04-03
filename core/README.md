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
basename        2021-04-03             569
cat             2021-04-03             1430
cat2            2021-04-03             1304
chroot          2021-04-03             620
dd              2021-04-03             5008
echo            2021-04-03             465
false           2021-04-03             214
getenv          2021-04-03             1142
gid             2021-04-03             2904
grep            2021-04-03             1104
head            2021-04-03             1528
kill            2021-04-03             1946
ln              2021-04-03             838
ls              2021-04-03             5448
ls2             2021-04-03             4752
mkdir           2021-04-03             4096
mkfifo          2021-04-03             888
mount           2021-04-03             1592
pivot_root      2021-04-03             346
printenv        2021-04-03             714
pwd             2021-04-03             338
rgrep           2021-04-03             6568
rm              2021-04-03             884
rmdir           2021-04-03             888
seq             2021-04-03             529
sleep           2021-04-03             520
stat            2021-04-03             2192
sync            2021-04-03             368
tail            2021-04-03             3032
tee             2021-04-03             2951
touch           2021-04-03             537
true            2021-04-03             211
umask           2021-04-03             487
umount          2021-04-03             1184
usleep          2021-04-03             472
waitfor         2021-04-03             3168
where           2021-04-03             1032
yes             2021-04-03             368
===============================================
                          size: 62637 Bytes
```
