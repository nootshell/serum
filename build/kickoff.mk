BUILD_TARGETS = $(foreach module,$(MODULES),$(module))




.PHONY: all $(BUILD_TARGETS)


all: $(BUILD_TARGETS)




include $(foreach module,$(MODULES),$(call GetModuleRoot,$(module))/build.mk)
