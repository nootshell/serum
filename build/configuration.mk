#
# Compiler/linker setup
#

CC = gcc
LD = gcc

# Global C flags
CFLAGS = \
	-fPIC -Wall \
	-Isrc \
	-DLS_EXPORTING=1 \
	-DTIMESTAMP='"$(TIMESTAMP)"'

# Profile-specific C flags
CFLAGS_DEBUG = -g -DDEBUG=1
CFLAGS_RELEASE = -O3

# Libraries to use
#LD_LIBS = pthread

# Parallelism
#PARALLELISM = full|friendly
PARALLELISM = friendly




#
# Choose an output mode
#

OUTPUT_MODE = raw

#OUTPUT_MODE = simple

#OUTPUT_MODE = silent

#OUTPUT_MODE = char
#OUTPUT_CHAR = .




#
# Define modules to build
#

MODULES = serum.core




#
# Computed variables, pls no touchy
#

CFLAGS_PROFILE = $(CFLAGS_$(call TransformProfileName,$(PROFILE)))

ifeq (full,$(PARALLELISM))
PARALLELISM_CORES := $(shell nproc --all)
else
PARALLELISM_CORES := $(shell nproc --ignore=1)
endif
