BUILD_OBJ = $(CC) $(CFLAGS) $(CFLAGS_PROFILE) -c $^ -o $(call TransformObjectName,$@);




obj/%.o: %.c
	@mkdir -p "$(@D)";
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
