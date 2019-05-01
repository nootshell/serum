OBJ_FILE = $(call TransformObjectName,$@)
BuildObject = $(CC) $(CFLAGS) $(CFLAGS_PROFILE) -c '$1' -o '$(OBJ_FILE)'
BuildSharedObject = $(LD) -o '$(call TransformSharedObjectName,$@)' -shared $(foreach obj,$1,'$(call TransformObjectName,$(obj))') $(foreach lib,$(LD_LIBS),-l$(lib))




obj/%.o: %.c
	@test ! -L '$@' && ln -s '$(shell mkdir -p '$(@D)' && touch '$(OBJ_FILE)' && realpath '$(OBJ_FILE)')' '$@';
ifeq (simple,$(OUTPUT_MODE))
	@echo "$^";
	@$(call BuildObject,$^)
else ifeq (char,$(OUTPUT_MODE))
	@$(call BuildObject,$^)
	@echo -n "$(OUTPUT_CHAR)";
else ifeq (silent,$(OUTPUT_MODE))
	@$(call BuildObject,$^)
else
	$(call BuildObject,$^)
endif
