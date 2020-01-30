Ongoing work. A soothing puzzle.

I'm trying to get a usable unix system (without the kernel)
within 64kB. All tools statically linked with (minilib)[minilib].

Ok. Possibly it's going to be 128kB.
But no more. 128kB ought to be enough for everycore. Eeh. Something like this.

We'll see. It's also the question of how many tools should be just "stacked".
E.g., instead of `ls -lrt`, essentially just piping the tools together: `ls | stat | sort`.
For example. I guess, awk might be the right tool at this place.
Thinking about it, sed might already be enough. 
At least for executing stat foreach ls-listed file.

For some of the gnu core tools, this is obviously the best solution.
e.g. `dir` should either be an alias to `ls -C -b`,
or, to stay with the single executables, 
there should be a simple sh script calling ls.
;) the script could even consist of only the first line: `#!/bin/ls -C -b`
What will invoke ls, with the according parameters.

I somehow like the idea of script wrappers more, 
than having symbolic links to multicall binaries.
Amongst other, it's easier to customize them.
And they are in a central directory. Aliases always tend to be spread 
over several config files, at least on my systems.


And, to say that clearly and repeatedly, it's a fun project. 
I just like to have a unix system within 64kB. ;)
The intention is not on being posix, systemV or whatever compatible.
And for the command line options, I'm also going to implement only 
those, I'm using mostly, or which don't add to much size.

There are other projects out there with a serious (what's the word?) 'approach'.
I personally like 'toybox' most.
Albite I also still believe, sort of a 'microkernel' is a better approach,
and toybox is monlithic. But that's a question, I'm not sure yet.
The advantages of an monolithic approach are undeniable there.

On the other hand, my experiences with small statically linked tools are
very good, as well. 
When about performance, small statically linked tools surprisingly are more responsive. 
When thinking about it, it's not so surprising. Only the needed "parts" of the code ar
e loaded,
the small tools (mainly less than 1kB fit even into the cpu's cache, 
they also are buffered in the memory's cache for the hd,
and there's no linker needed.
Also, dropping unicode, language, and whatever support is no loss,
at least not for me. 
Who in the world would need unicode support for system development/programming/..?
Ok. That's about glibc, and also musl et al.
But they also do have other targets.

---

When looking for something usable, and you do not need only single tools,
you're most possibly better with something more mature and more serious.
I personally like the toybox project very much. 
Its mature, has a quite stringent posix implementation, and is also below 
1MB overall.

http://github.com/toybox

---


For compiling, you'll need the script mini-gcc from minilib.
~What, again, reminds me of my plan to put the whole minilib into
this script. No installing anymore, no version problems, just 
one wrapper script mini-gcc.¨

Done. `curl http://github.com/ ..TODO ` and you're good to go.




(Memo to me:) basic unix core tools could be a great testcase for minilib:
 compile them, and then let them execute a test script.
 the needed scripts are already implemented in minilib/test/




The sources are partially written completely new, 
partially scratched together from: suckless base utils, minutils, minix.

I keep all copyright notices within the sources.

Please, don't get this project too serious. 
It's more sort of a soothing puzzle to me.

---

### Tools working so far:

```
basename        2019-08-19             493
cat             2019-08-20             1038
echo            2020-01-19             368
false           2019-08-19             153
getenv          2019-08-19             782
head            2019-08-20             1407
ls              2020-01-15             13344
pwd             2020-01-19             305
tail            2019-08-20             2576
tee             2019-08-19             2335
touch           2020-01-19             464
true            2019-08-19             150
uname           2020-01-19             1362
yes             2020-01-15             309
===============================================
                          size: 25086 Bytes
```
