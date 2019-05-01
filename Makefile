include build/configuration.mk

# If nothing is specified, automagically run parallel
parallel:
	@echo Running with -j$(PARALLELISM_CORES)
	@$(MAKE) -f build/bootstrap.mk --no-print-directory -j$(PARALLELISM_CORES)

include build/bootstrap.mk
