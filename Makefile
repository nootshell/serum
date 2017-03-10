CC = gcc
LD = gcc

CFLAGS = -fPIC -fstack-protector-strong -I.



all: release test

all_dbg: debug test

clean:
	@echo cleanup
	@rm -R bin obj



obj/%.o: %.c
	@echo $@
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $^ -o $@



bin/libserum.so: CFLAGS += -DLIBSERUM_EXPORTS
bin/libserum.so: $(addprefix obj/, $(patsubst %.c, %.o, $(shell find libserum -type f -name '*.c')))
	@echo $@
	@mkdir -p $(@D)
	@$(LD) -o $@ -shared $^

bin/test: bin/libserum.so
bin/test: $(addprefix obj/, $(patsubst %.c, %.o, $(shell find test -type f -name '*.c')))
	@echo $@
	@mkdir -p $(@D)
	@$(CC) -lserum -o $@ $^



debug: CFLAGS += -g -DDEBUG
debug: lib

release: CFLAGS += -O3 -DRELEASE
release: lib



lib: bin/libserum.so

test: bin/test
