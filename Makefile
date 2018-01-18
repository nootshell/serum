CC = gcc
LD = gcc

CFLAGS = \
	-fPIC -fstack-protector-strong -I. \
	-DMAKEFILE=1


all: release test


clean:
	@echo -n "Cleaning..."
	@rm -f bin/libserum.so
	@rm -f bin/test
	@rm -Rf obj/libserum
	@rm -Rf obj/test
	@echo " done."
	@echo

obj/%.o: %.c
	@echo -n "| $@"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $^ -o $@
	@echo " (done)"



bin/libserum.so: CFLAGS += -DLIBSERUM_EXPORTS=1
bin/libserum.so: $(addprefix obj/, $(patsubst %.c, %.o, $(shell find libserum -type f -name '*.c')))
	@echo -n "+-> $@"
	@mkdir -p $(@D)
	@$(LD) -o $@ -shared $(LS_MCHECK) $(LS_PTHREADS) -Wl,-e,interp_entry $^
	@echo " (done)"
	@echo

#bin/test: bin/libserum.so
bin/test: $(addprefix obj/, $(patsubst %.c, %.o, $(shell find test -type f -name '*.c')))
	@echo -n "+-> $@"
	@mkdir -p $(@D)
	@$(CC) -o $@ $^
	@echo " (done)"
	@echo



debug: CFLAGS += -g -DDEBUG
debug: bin/libserum.so

release: CFLAGS += -O3 -DRELEASE
release: bin/libserum.so

