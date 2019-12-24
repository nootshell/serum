#!/usr/bin/env -S make -f

CC = gcc
LD = gcc

PROJECT_NAME = serum
PROJECT_SOURCE = ./src
PROJECT_DIST = ./dist

CFLAGS_EXTRA = -DSERUM_BUILD_MUTE=1 -Wpedantic -Werror=all

DEFAULT_TARGET = libserum

include build/makeframe/bootstrap.mk




libserum: $(PROJECT_DIST)/libserum.so

$(PROJECT_DIST)/libserum.so: $(OBJECTS)
	$(LD) -shared $(LFLAGS) -o $@ $^
