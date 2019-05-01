OBJ_FILE = $(call TransformObjectName,$@)
BUILD_OBJ = $(CC) $(CFLAGS) $(CFLAGS_PROFILE) -c '$^' -o '$(OBJ_FILE)'
BUILD_SO = $(LD) -o '$(call TransformSharedObjectName,$@)' -shared $(foreach obj,$^,'$(call TransformObjectName,$(obj))') $(foreach lib,$(LD_LIBS),-l$(lib))




obj/%.o: %.c
	@test ! -L '$@' && ln -s '$(shell mkdir -p '$(@D)' && touch '$(OBJ_FILE)' && realpath '$(OBJ_FILE)')' '$@';
ifeq (simple,$(OUTPUT_MODE))
	@$(BUILD_OBJ)
	@echo "$^";
else ifeq (char,$(OUTPUT_MODE))
	@$(BUILD_OBJ)
	@echo -n "$(OUTPUT_CHAR)";
else ifeq (silent,$(OUTPUT_MODE))
	@$(BUILD_OBJ)
else
	$(BUILD_OBJ)
endif
