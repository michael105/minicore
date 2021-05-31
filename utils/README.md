### Static builds for linux amd64

Compiled and linked with the standard toolchain, gcc and ld,  -Os;

Here go tools, not neccessarily present at a standard linux system.

Since I'm aiming at a 64kB (128kB max) base system, I've to separate them.


```
ap              2021-05-31             1320
conv            2021-05-31             3144
errno           2021-05-31             7837
fromhex         2021-05-31             816
getresuid       2021-05-31             2448
kgetty          2021-05-31             3832
kgetty-persistent 2021-05-31             4864
ksudo           2021-05-31             924
lockfile        2021-05-31             3776
nbdo            2021-05-31             983
sed/sed         2020-07-07             16748
su              2021-05-31             661
tohex           2021-05-31             840
udo             2021-05-31             1252
xorpipe         2021-05-31             4464
===============================================
                          size: 53909 Bytes
```
