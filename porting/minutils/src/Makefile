
onlytext=yes echo

textandbss=pwd cat tee

PROG=

ifndef MINI_GCC
		MINI_GCC=mini-gcc
endif




ifdef MAKEONLYTEXT
$(MAKEONLYTEXT): $(MAKEONLYTEXT).c $(MAKEONLYTEXT).mconf
	$(MINI_GCC) --shrinkelf --ldscript onlytext --config $(MAKEONLYTEXT).mconf -include minilib.h -DINCLUDESRC -o $(MAKEONLYTEXT) $(MAKEONLYTEXT).c 
	

$(MAKEONLYTEXT).mconf:
	$(MINI_GCC) --shrinkelf --no-mbuf --genconf $(MAKEONLYTEXT).mconf $(MAKEONLYTEXT).c

else

ifdef MAKETEXTANDBSS
$(MAKETEXTANDBSS): $(MAKETEXTANDBSS).c $(MAKETEXTANDBSS).mconf
	$(MINI_GCC) --shrinkelf --ldscript text_and_bss --config $(MAKETEXTANDBSS).mconf -include minilib.h -DINCLUDESRC -o $(MAKETEXTANDBSS) $(MAKETEXTANDBSS).c 
	

$(MAKETEXTANDBSS).mconf:
	$(MINI_GCC) --shrinkelf --no-mbuf --genconf $(MAKETEXTANDBSS).mconf $(MAKETEXTANDBSS).c

else



ifdef BUILDPROG
$(BUILDPROG): $(BUILDPROG).c $(BUILDPROG).mconf
	$(MINI_GCC) --shrinkelf --config $(BUILDPROG).mconf -include minilib.h -DINCLUDESRC -o $(BUILDPROG) $(BUILDPROG).c 
	


$(BUILDPROG).mconf:
	$(MINI_GCC) --shrinkelf --genconf $(BUILDPROG).mconf $(BUILDPROG).c

else



all:
	@$(foreach P,$(onlytext),make MAKEONLYTEXT=$(P); )
	@$(foreach P,$(PROG),make BUILDPROG=$(P); )
	@$(foreach P,$(textandbss),make MAKETEXTANDBSS=$(P); )

clean:
	@echo rm $(onlytext) $(PROG) $(textandbss)
	rm -f $(onlytext) $(PROG) $(textandbss) 

	
#@$(foreach P,$(onlytext) $(PROG) $(textandbss), rm $P; )

endif
endif
endif

