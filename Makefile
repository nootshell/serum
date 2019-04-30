PROJECTS = libserum test
PROFILE = debug
OUTPUT_MODE = raw

include .make/base.mk

rebuild: clean all

fast-libserum:
	@$(MAKE) --no-print-directory -j8 OUTPUT_MODE=dot libserum

fast-test:
	@$(MAKE) --no-print-directory -j8 OUTPUT_MODE=dot test

fast: fast-libserum fast-test
