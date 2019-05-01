#
# Compiler/linker selection
#

CC = gcc
LD = gcc




#
# Define C flags
#

# Global flags
CFLAGS = \
	-fPIC -Wall \
	-Isrc \
	-DLS_EXPORTING=1

# Profile-specific flags
CFLAGS_DEBUG = -g -DDEBUG=1
CFLAGS_RELEASE = -O3




#
# Choose an output mode
#

#OUTPUT_MODE = raw

#OUTPUT_MODE = simple

OUTPUT_MODE = silent

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
