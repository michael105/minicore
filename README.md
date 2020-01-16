Ongoing work. 

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

---

For compiling, you'll need the script mini-gcc from minilib.
What, again, reminds me of my plan to put the whole minilib into
this script. No installing anymore, no version problems, just 
one wrapper script mini-gcc. 




(Memo to me:) basic unix core tools could be a great testcase for minilib:
 compile them, and then let them execute a test script.
 the needed scripts are already implemented in minilib/test/




The sources are partially written completely new, 
partially scratched together from: suckless base utils, minutils, minix.

I keep all copyright notices within the sources.

