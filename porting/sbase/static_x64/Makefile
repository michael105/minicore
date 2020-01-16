
.PHONY: README.md

README.md: 
	cp README.in README.md
	ls -lho --time-style=long-iso | grep -oE '\s*(\S*\s*){5}$$' >> README.md
	echo '```'>> README.md

