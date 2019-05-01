serum.crypto: $(call GetModuleObjects,.c,$@)
	$(call BuildSharedObject,$(call GetModuleObjects,.c,$@))
