TransformProfileName = $(shell echo "$1" | sed 's/[[:space:]\-]/_/g' | tr [:lower:] [:upper:] | sed 's/[^A-Za-z0-9_]//g')
TransformObjectName = $(subst src.,,$(subst obj.,obj/,$(subst /,.,$1)))
TransformSharedObjectName = bin/lib$1.so

GetModuleRoot = src/$(subst .,/,$1)
GetAllObjects = $(addprefix obj/,$(patsubst %$1,%.o,$(shell find $2 -type f -name '*.c')))
GetModuleObjects = $(call GetAllObjects,$1,$(call GetModuleRoot,$2))
