.PHONY: all force clean

C_SOURCES := $(wildcard src/*.c)
C_TARGETS := $(patsubst src/%.c,bin/%,$(C_SOURCES))

S_SOURCES := $(wildcard src/*.s)
S_TARGETS := $(patsubst src/%.s,bin/%,$(S_SOURCES))

TARGETS   := $(C_TARGETS) $(S_TARGETS)

# FLAGS     := -Wall -Wextra -Wshadow -Wmissing-prototypes

all: CFLAGS += -Os
all: bin $(TARGETS)

debug: CFLAGS += -g
debug: bin $(TARGETS)

bin:
	@echo '[MD] bin'
	@mkdir -p bin

bin/%: src/%.c
	if [[ ! -L $< ]]; then \
		echo '[CC] $(@F)'; \
		$(CC) $(FLAGS) $(CFLAGS) -o $@ $<; \
	else \
		echo '[LN] $(@F)'; \
		source="$$(readlink '$<' | cut -d. -f1)"; \
		cd bin; \
		ln -sf $${source} $(@F); \
	fi

bin/%: src/%.s
	@echo '[AS] $(@F)'
	$(CC) -s -nostdlib -o $@ $<

bin/cmp: src/cmp.c
	@echo '[CC] $(@F)';
	$(CC) $(FLAGS) $(CFLAGS) -Wno-implicit-fallthrough -o $@ $<

clean:
	@echo '[RM] bin'
	@rm -rf bin

