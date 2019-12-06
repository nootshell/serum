#!/usr/bin/env -S make -f

PROJECT_NAME = serum
PROJECT_SOURCE = ./src
PROJECT_DIST = ./dist
CFLAGS_EXTRA = -fno-diagnostics-show-caret

DEFAULT_TARGET = libserum

include build/makeframe/bootstrap.mk




libserum: $(PROJECT_DIST)/libserum.so

$(PROJECT_DIST)/libserum.so: $(OBJECTS)
	$(LD) -shared $(LFLAGS) -o $@ $^
