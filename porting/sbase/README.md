# suckless linux tools


This is my fork of github.com/nee-san/sbase

Working on a port to minilib,
in order to get tiny and statical linked binaries.


Seems quite useful, linked static with minilib on linux x64
"cat" now is 3K.

	same system, 
	linked dynamic with gcc: 15K (18K)
	linked static with gcc: 747K
	dynamic with musl: 17K
	static with musl: 26K (or even 32K)



## Notes

Although I don't see why,
including the sources directly (instead of building the whole libutil)
seems to produce slightly bigger binaries.

ATM, something bloats true.c and false.c.
205Bytes for the binaries.(??)
compared with 151 Bytes I got with "hello world", there are somehow 54 Bytes to much.
67 Bytes, if you count the string "hello world!".
Annoying.
Dunno.
Have to sort this out. **I DONT LIKE BLOATWARE!!!**



;) But porting the single binaries one after another is sort of a soothing puzzle to me,
so I'll go with including.

Need to put ifndefs into the sources.




Michael (misc) Myer, misc.myer@zoho.com, 2019

