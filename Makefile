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




#
#	Project variables
#

SUBPROJECTS = libserum test
PROFILE = debug-cst

CFLAGS_DEBUG = \
	-g -DLS_DEBUG=1

CFLAGS_DEBUG_CST = \
	$(CFLAGS_DEBUG) -DLSCST_ENABLED=1

CFLAGS_RELEASE = \
	-Ofast

CFLAGS_RELEASE_CST = \
	$(CFLAGS_RELEASE) -DLSCST_ENABLED=1




#
#	Compiler setup
#

CC =

ifeq ($(strip $(CC)),)
	CC = tcc
endif

CFLAGS = \
	-Wall -Wpadded -fPIC $(call ifoptavail,$(CC),-fstack-protector-strong) -I. \
	-DMAKEFILE=1 -DTIMESTAMP="\"$(shell date -u +'%Y-%m-%dT%H:%M:%S%:z')\"" \
	-DGIT_BRANCH="\"$(GIT_BRANCH)\"" -DGIT_TAG="\"$(GIT_TAG)\"" \
	-DFILEPATH="\"$^\"" -DKERNEL="\"$(KERNEL)\"" -DKERNEL_ARCH="\"$(KERNEL_ARCH)\"" \
	-DLS_ANSI_SUPPORT=1 $(CFLAGS_EXTRA)

#
#	Versioning setup
#

GIT_COMMIT = $(shell git rev-parse --short HEAD 2>/dev/null || echo "unknown")
GIT_BRANCH = $(shell git rev-parse --short --abbrev-ref HEAD 2>/dev/null || echo "unknown")
GIT_TAG = $(shell git describe --tags --abbrev=0 2>/dev/null || echo "tagless")

INCR_NUM = $(eval INCR_NUM_VAL = $(shell if [ -z "$(INCR_NUM_VAL)" ]; then ./.make/incr.sh; else echo "$(INCR_NUM_VAL)"; fi))$(INCR_NUM_VAL)




#
#	Internal variables
#

ECHO_FLAG = -n
ECHO_EOL = "$(ECHO_FLAG)$(eval ECHO_FLAG =)"

ifeq (,$(TITLE))
	TITLE = $(PROFILE);
endif

tvn = $(shell echo "$1" | sed 's/[[:space:]\-]/_/g' | tr [:lower:] [:upper:] | sed 's/[^A-Za-z0-9_]//g')
getfiles = $(addprefix obj/, $(patsubst %.c, %.o, $(shell find $1 -type f -name '*.c')))
ifoptavail = $(shell .make/cc/ifoptavail.sh $1 $2 $3 $4 $5)

CFLAGS_PROFILE = $(CFLAGS_$(call tvn,$(PROFILE)))
PROJECT_TARGETS = $(foreach proj,$(SUBPROJECTS),pre-$(proj) $(proj) post-$(proj))




#
#	Phonies
#		make-debug: To debug this Makefile.
#		clean: To clean up bin/ and obj/.
#		all: Default built target to build everything.
#

.PHONY: make-debug clean all default reset-incr $(PROJECT_TARGETS)

default: all




#
#	Target: make-debug
#

make-debug:
	$(eval ECHO_BAK = $(ECHO_FLAG))
	@echo $(strip $(ECHO_EOL));
	@echo "Section newline: $(shell if [ '$(ECHO_BAK)' = '-n' ]; then echo 'no'; else echo 'yup'; fi;)";
	@echo "Projects: $(SUBPROJECTS)";
	@echo " - Targets: $(PROJECT_TARGETS)";
	@echo "Profile: $(PROFILE) (CFLAGS_$(call tvn,$(PROFILE)))";
	@echo "Compiler: $(CC)";
	@echo "Flags: $(CFLAGS)";
	@echo " - Profile: $(CFLAGS_PROFILE)";
	@echo "Git:";
	@echo " - Tag: $(GIT_TAG)";
	@echo " - Commit: $(GIT_COMMIT)";
	@echo " - Branch: $(GIT_BRANCH)";
	@echo "Incremental num.: $(INCR_NUM)";




#
#	Target: clean
#

clean:
	@echo $(strip $(ECHO_EOL))
	@./.make/clean.sh $(strip $(SUBPROJECTS));




#
#	Target: all
#

all: $(foreach proj,$(SUBPROJECTS),$(proj))




#
#	Target: reset-incr
#

reset-incr:
	@rm -f ./.make/incr




#
#	Target: glob
#

obj/%.o: %.c
	@mkdir -p $(@D);
	@echo -n "> $(CC) $^";
	@$(CC) -o $@ $(CFLAGS) $(CFLAGS_PROFILE) -c $^;
	@echo " (done)";




#
#	Implement your projects below here.
#

###############################################################################




bin/libserum.so: CFLAGS += -DLS_EXPORTING=1
bin/libserum.so: $(call getfiles,libserum)
	@echo -n "=> $@";
	@mkdir -p $(@D);
	@$(CC) -o $@ -shared $^ -lpthread
	@echo " (done)";



bin/test: $(call getfiles,test)
	@echo -n "=> $@";
	@mkdir -p $(@D);
	@$(CC) -o $@ $^ $(shell ((test -f bin/libserum.so && realpath bin/libserum.so) || echo '-lserum'));
	@echo " (done)";



pre-libserum:
	@echo $(strip $(ECHO_EOL))

libserum: pre-libserum bin/libserum.so




pre-test:
	@echo $(strip $(ECHO_EOL))

test: pre-test bin/test




install: libserum
	@echo $(strip $(ECHO_EOL))
	@echo -n "> mv bin/libserum.so /usr/lib/libserum.so";
	@mv bin/libserum.so /usr/lib/libserum.so;
	@echo " (done)";
