###############################################################################
##                                                                           ##
##   The MIT License                                                         ##
##                                                                           ##
##   Copyright 2017-2018 icecubetray                                         ##
##                                                                           ##
##   Permission is hereby granted, free of charge, to any person             ##
##   obtaining a copy of this software and associated documentation files    ##
##   (the "Software"), to deal in the Software without restriction,          ##
##   including without limitation the rights to use, copy, modify, merge,    ##
##   publish, distribute, sublicense, and/or sell copies of the Software,    ##
##   and to permit persons to whom the Software is furnished to do so,       ##
##   subject to the following conditions:                                    ##
##                                                                           ##
##   The above copyright notice and this permission notice shall be          ##
##   included in all copies or substantial portions of the Software.         ##
##                                                                           ##
##   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,         ##
##   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF      ##
##   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  ##
##   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    ##
##   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    ##
##   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       ##
##   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  ##
##                                                                           ##
###############################################################################



CC = gcc
LD = gcc

LS_ELF_INTERP = $(shell ./elf_interp.sh)

GIT_COMMIT_HEAD = $(shell git rev-parse HEAD)
GIT_BRANCH = $(shell git rev-parse --abbrev-ref HEAD)
GIT_TAG = $(shell git describe --tags --abbrev=0 2> /dev/null || echo "0-dev")
TIMESTAMP = $(shell date -Iseconds)
KERNEL = $(shell uname -sr)
KERNEL_ARCH = $(shell uname -m)

CFLAGS = \
	-Wall -fPIC -fstack-protector-strong -I. \
	-DMAKEFILE=1 \
	-DGIT_BRANCH="\"$(GIT_BRANCH)\"" -DGIT_TAG="\"$(GIT_TAG)\"" \
	-DFILEPATH="\"$^\"" -DTIMESTAMP="\"$(TIMESTAMP)\"" -DKERNEL="\"$(KERNEL)\"" -DKERNEL_ARCH="\"$(KERNEL_ARCH)\""

TITLE = "done"



.PHONY: all clean debug release rebuild



ifeq (,$(wildcard ./debug))
all: release
rebuild: clean release
else
all: rebuild
rebuild: clean debug
endif



obj/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -DGIT_COMMIT="\"$(shell git log -n 1 --pretty=format:" ($(GIT_BRANCH) %H %aI)" -- $^)\"" -c $^ -o $@
	@echo "| $@ (done)"

obj/libserum/core/main.o: $(shell touch libserum/core/main.c)
obj/libserum/core/main.o: CFLAGS += $(LS_ELF_INTERP) -Wno-unused-command-line-argument
obj/libserum/core/main.o: libserum/core/main.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -DGIT_COMMIT="\"$(shell git log -n 1 --pretty=format:" ($(GIT_BRANCH) %H %aI)" -- $^)\"" -c $^ -o $@
	@echo "| $@ (done)"



bin/libserum.so: CFLAGS += -DLS_EXPORTING=1
bin/libserum.so: obj/libserum/core/main.o $(addprefix obj/, $(patsubst %.c, %.o, $(shell find libserum -type f -name '*.c' ! -wholename libserum/core/main.c)))
	@echo -n "+-> $@"
	@mkdir -p $(@D)
	@$(LD) -o $@ -shared -lpthread $(LS_ELF_INTERP) $^
	@echo -n " ($(TITLE))$(shell ./autodoxy.sh wrap)"
	@echo

bin/test: bin/libserum.so
bin/test: $(addprefix obj/, $(patsubst %.c, %.o, $(shell find test -type f -name '*.c')))
	@echo -n "+-> $@"
	@mkdir -p $(@D)
	@$(CC) -o $@ $^
	@echo " ($(TITLE))"
	@echo



clean:
	@rm -f bin/libserum.so
	@rm -f bin/test
	@rm -Rf obj/libserum
	@rm -Rf obj/test

debug: TITLE = "debug"
debug: CFLAGS += -g -DDEBUG
debug: bin/libserum.so

release: TITLE = "release"
release: CFLAGS += -O3 -DRELEASE
release: bin/libserum.so
