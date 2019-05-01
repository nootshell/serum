serum.core: $(call GetModuleObjects,.c,$@)
	$(call BuildSharedObject,$(call GetModuleObjects,.c,$@))
