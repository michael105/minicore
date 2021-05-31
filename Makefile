

all: README.md
	cd core 
	$(make)
	cd core 
	$(make)


up: all
	git commit -m 'update' -a
	git push



.PHONY: README.md

README.md: 
	cp README.in README.md
	#ls -lho --time-style=long-iso | grep -oE '\s*(\S*\s*){5}$$' >> README.md
	#echo '```'>> README.md
	cd core && perl Makeindex.pl >> ../README.md
	echo '```'>> README.md

