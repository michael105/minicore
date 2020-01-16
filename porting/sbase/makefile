# A template makefile 
# Set Prog to the program's name
# unpack minilib to the "source dir/minilib"
# or set a link
# copy this makefile into the source dir
# done.

# Or: download the file minilibcombined.c
# have a look in minilib.h, which defines you need
# define your switches, and include 
# minilibcombined.c into one of your sourcefiles.
# 



ifndef PROG
PROG=yes true false pwd wc
endif

# where to build object files (defaults to ./ )
BUILDDIR=build

# where should the binares go?
BINDIR=./static_x64

# Where minilib is installed, or should be installed to
MLIBDIR=minilib

# Don't create obj files, include evrything in one gcc run.(yourself)
#SINGLERUN=1

# Include everything yourself
NOINCLUDE=1

# Include the sources of minilib with the compat headers
# (e.g., build the source of printf within the header stdio.h)
#INCLUDECOMPATSRC=1

# include only the Text section in the resulting elf output
#ONLYTEXT= true false yes 

# Set the optimization flag (default: -O1)
# Be careful with that, -Os as well as -O2 seem to be problematic
# Maybe more functions need to be volatile ?
#
OPTFLAG=-Os

# GCC
#GCC=gcc

# LD
#LD=ld


# Truncate elf binaries
SHRINKELF=1

# Report verbose 
#VERBOSE=1

# configsettings ending here


ifdef BUILDPROG
		PROG=$(BUILDPROG)
		include $(MLIBDIR)/Makefile.include
else


#( This is a recursive makefile and will call itself with this param)
ifdef with-minilib
include $(MLIBDIR)/Makefile.include 
endif

all:
	@$(foreach P,$(PROG),$(MAKE) BUILDPROG=$(P) OFILES=libutil && ) true
	cd static_x64 && make


#Compile with minilib or without
# Will compile with minilib, if present and found in MLIBDIR
default: 
	$(if $(wildcard $(MLIBDIR)/Makefile.include), make with-minilib=1, make native )


# build without minilib
# compile with dynamic loading, os native libs
native: $(PROG).c
	$(info call "make getminilib" to fetch and extract the "minilib" and compile $(PROG) static (recommended) )
	gcc -o $(PROG) $(PROG).c


# we haven't been callen recursive
ifndef with-minilib

ifndef BUILDDIR
		BUILDDIR=.
endif

rebuild:
	make clean
	make

clean:
	$(shell rm $(PROG))
	$(shell cd $(BUILDDIR) && rm *.o)

endif

getminilib: $(MLIBDIR)/minilib.h

$(MLIBDIR)/minilib.h:
	$(info get minilib)
	git clone https://github.com/michael105/minilib.git $(MLIBDIR) || (( (curl https://codeload.github.com/michael105/minilib/zip/master > minilib.zip) || (wget https://codeload.github.com/michael105/minilib/zip/master)) && unzip minilib.zip && mv minilib-master $(MLIBDIR))
	make rebuild

install: $(PROG)
	cp $(PROG) /usr/local/bin

endif 
#(ifdef BUILDPROG)
