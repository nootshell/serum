include build/functions.mk build/configuration.mk

# If nothing is specified, automagically run parallel
parallel:
	@echo Running with -j$(PARALLELISM_CORES)
	@$(MAKE) -f build/bootstrap.mk --no-print-directory -j$(PARALLELISM_CORES) all

include build/variables.mk build/compilation.mk build/kickoff.mk
