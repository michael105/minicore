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
basename        2022-05-10             1980
cat             2022-05-10             2845
cat2            2022-05-10             2519
chroot          2022-05-10             621
cksum           2022-05-10             2640
crc             2022-05-10             784
date            2022-05-10             2565
dd              2022-05-10             6320
echo            2022-05-10             466
false           2022-05-10             215
getenv          2022-05-10             2349
gid             2022-05-10             2772
grep            2022-05-10             1144
head            2022-05-10             1671
help            2022-05-10             544
id              2022-05-10             5584
kill            2022-05-10             3090
ln              2022-05-10             2053
loadplugin      2022-05-10             4472
ls              2022-05-10             5152
ls2             2022-05-10             5106
mkfifo          2022-05-10             2071
mount           2022-05-10             2832
pivot_root      2022-05-10             347
printenv        2022-05-10             1889
printf          2022-05-10             2976
pwd             2022-05-10             635
rgrep           2022-05-10             6880
rm              2022-05-10             2067
rmdir           2022-05-10             2071
seq             2022-05-10             529
sleep           2022-05-10             500
stat            2022-05-10             5280
sync            2022-05-10             369
tail            2022-05-10             3320
tee             2022-05-10             4214
touch           2022-05-10             720
true            2022-05-10             212
umask           2022-05-10             484
umount          2022-05-10             2328
usleep          2022-05-10             458
waitfor         2022-05-10             3693
where           2022-05-10             1258
where2          2022-05-10             1068
yes             2022-05-10             348
===============================================
                         size: 101441 Bytes
```
