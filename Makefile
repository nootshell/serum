CC = gcc
LD = gcc

DISTRO_DEF = $(shell sh ./get-distro.sh)
CFLAGS = -fPIC -fstack-protector-strong -I. -DMAKEFILE=1 -DDISTRO=\"$(DISTRO_DEF)\"



all: release test

all_dbg: debug test

clean:
	@echo -n "Cleaning..."
	@rm -f bin/libserum.so
	@rm -f bin/test
	@rm -Rf obj/libserum
	@rm -Rf obj/test
	@echo " done."
	@echo

r: clean debug test



obj/%.o: %.c
	@echo -n "| $@"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $^ -o $@
	@echo " (done)"



bin/libserum.so: CFLAGS += -DLIBSERUM_EXPORTS=1
bin/libserum.so: $(addprefix obj/, $(patsubst %.c, %.o, $(shell find libserum -type f -name '*.c')))
	@echo -n "+-> $@"
	@mkdir -p $(@D)
	@$(LD) -o $@ -shared $^
	@echo " (done)"
	@echo

bin/test: bin/libserum.so
bin/test: $(addprefix obj/, $(patsubst %.c, %.o, $(shell find test -type f -name '*.c')))
	@echo -n "+-> $@"
	@mkdir -p $(@D)
	@$(CC) -lserum -o $@ $^
	@echo " (done)"
	@echo



debug: CFLAGS += -g -DDEBUG
debug: lib

release: CFLAGS += -O3 -DRELEASE
release: lib



lib: bin/libserum.so

test: bin/test
