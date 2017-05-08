/*******************************************************************************
**                                                                            **
**   The MIT License                                                          **
**                                                                            **
**   Copyright 2017 icecubetray                                               **
**                                                                            **
**   Permission is hereby granted, free of charge, to any person              **
**   obtaining a copy of this software and associated documentation files     **
**   (the "Software"), to deal in the Software without restriction,           **
**   including without limitation the rights to use, copy, modify, merge,     **
**   publish, distribute, sublicense, and/or sell copies of the Software,     **
**   and to permit persons to whom the Software is furnished to do so,        **
**   subject to the following conditions:                                     **
**                                                                            **
**   The above copyright notice and this permission notice shall be           **
**   included in all copies or substantial portions of the Software.          **
**                                                                            **
**   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          **
**   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       **
**   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   **
**   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     **
**   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     **
**   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        **
**   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   **
**                                                                            **
********************************************************************************
**
**  Notes:
**    -
**
*/

#define FILE_PATH							"core/info.c"

#include "./info.h"
#include "./detect.h"
#include "./intrinsics.h"
#include "./identification.h"


#define OPTION_NOT_DEFINED					"[not defined]"


ID("compile-time build information");


const char*
ls_info_compilation_environment() {
	return "[" __DATE__ ", " __TIME__ "] (" LS_MAKEFILE_STRING ") " LS_COMPILER_STRING LS_INTRINSICS_STRING " " LS_COMPILER_VERSION " @ " LS_OS_STRING " (" LS_ARCH_STRING ", " LS_ENDIANNESS_STRING ")";
}


const char*
ls_info_compilation_options() {
	return
#if (STORE_COMPILATION_OPTIONS)
		"DEBUG                             "
#	if (defined(DEBUG))
		MACRO_STRINGIFY(DEBUG)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nDISTRO                            "
#	if (defined(DISTRO))
		MACRO_STRINGIFY(DISTRO)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nELF_INTERP                        "
#	if (defined(ELF_INTERP))
		MACRO_STRINGIFY(ELF_INTERP)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nGIT_BRANCH                        "
#	if (defined(GIT_BRANCH))
		MACRO_STRINGIFY(GIT_BRANCH)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nGIT_COMMIT                        "
#	if (defined(GIT_COMMIT))
		MACRO_STRINGIFY(GIT_COMMIT)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLIBSERUM_EXPORTS                  "
#	if (defined(LIBSERUM_EXPORTS))
		MACRO_STRINGIFY(LIBSERUM_EXPORTS)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_ANSI_ESCAPE_SUPPORT            "
#	if (defined(LS_ANSI_ESCAPE_SUPPORT))
		MACRO_STRINGIFY(LS_ANSI_ESCAPE_SUPPORT)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_DEVICE_BUFFER_BLOCK            "
#	if (defined(LS_DEVICE_BUFFER_BLOCK))
		MACRO_STRINGIFY(LS_DEVICE_BUFFER_BLOCK)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_ISO9797_ALLOW_ALL_ZERO         "
#	if (defined(LS_ISO9797_ALLOW_ALL_ZERO))
		MACRO_STRINGIFY(LS_ISO9797_ALLOW_ALL_ZERO)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_ISO9797_DENY_SIZE_ZERO         "
#	if (defined(LS_ISO9797_DENY_SIZE_ZERO))
		MACRO_STRINGIFY(LS_ISO9797_DENY_SIZE_ZERO)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_LOG_ORIGIN                     "
#	if (defined(LS_LOG_ORIGIN))
		MACRO_STRINGIFY(LS_LOG_ORIGIN)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_LOG_RESULTS                    "
#	if (defined(LS_LOG_RESULTS))
		MACRO_STRINGIFY(LS_LOG_RESULTS)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_MD5_DENY_SIZE_ZERO             "
#	if (defined(LS_MD5_DENY_SIZE_ZERO))
		MACRO_STRINGIFY(LS_MD5_DENY_SIZE_ZERO)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_NO_CHECK_INCLUDE_ASSUME_EXISTS "
#	if (defined(LS_NO_CHECK_INCLUDE_ASSUME_EXISTS))
		MACRO_STRINGIFY(LS_NO_CHECK_INCLUDE_ASSUME_EXISTS)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_RDTSC_NANOS_FAILSAFE           "
#	if (defined(LS_RDTSC_NANOS_FAILSAFE))
		MACRO_STRINGIFY(LS_RDTSC_NANOS_FAILSAFE)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_RIJNDAEL_STRICT                "
#	if (defined(LS_RIJNDAEL_STRICT))
		MACRO_STRINGIFY(LS_RIJNDAEL_STRICT)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_SALSA20_STRICT                 "
#	if (defined(LS_SALSA20_STRICT))
		MACRO_STRINGIFY(LS_SALSA20_STRICT)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_SELFTEST                       "
#	if (defined(LS_SELFTEST))
		MACRO_STRINGIFY(LS_SELFTEST)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_SELFTEST_CRYPTO_HASHING        "
#	if (defined(LS_SELFTEST_CRYPTO_HASHING))
		MACRO_STRINGIFY(LS_SELFTEST_CRYPTO_HASHING)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_SELFTEST_STARTUP               "
#	if (defined(LS_SELFTEST_STARTUP))
		MACRO_STRINGIFY(LS_SELFTEST_STARTUP)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_SELFTEST_VERBOSE               "
#	if (defined(LS_SELFTEST_VERBOSE))
		MACRO_STRINGIFY(LS_SELFTEST_VERBOSE)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_SHA2_DENY_SIZE_ZERO            "
#	if (defined(LS_SHA2_DENY_SIZE_ZERO))
		MACRO_STRINGIFY(LS_SHA2_DENY_SIZE_ZERO)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_SWAP_USE_INTRINSICS            "
#	if (defined(LS_SWAP_USE_INTRINSICS))
		MACRO_STRINGIFY(LS_SWAP_USE_INTRINSICS)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_USE_CHECK_BUILTIN              "
#	if (defined(LS_USE_CHECK_BUILTIN))
		MACRO_STRINGIFY(LS_USE_CHECK_BUILTIN)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_USE_CHECK_INCLUDE              "
#	if (defined(LS_USE_CHECK_BUILTIN))
		MACRO_STRINGIFY(LS_USE_CHECK_BUILTIN)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nLS_VERBOSE_STARTUP                "
#	if (defined(LS_VERBOSE_STARTUP))
		MACRO_STRINGIFY(LS_VERBOSE_STARTUP)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nMAKEFILE                          "
#	if (defined(MAKEFILE))
		MACRO_STRINGIFY(MAKEFILE)
#	else
		OPTION_NOT_DEFINED
#	endif

		"\nRELEASE                           "
#	if (defined(RELEASE))
		MACRO_STRINGIFY(RELEASE)
#	else
		OPTION_NOT_DEFINED
#	endif
#else
		"Compilation options not stored.\n"
		"To do so, recompile with: -DSTORE_COMPILATION_OPTIONS=1"
#endif
		;
}


const char*
ls_info_git_branch() {
#if (defined(GIT_BRANCH))
	return GIT_BRANCH;
#else
	return OPTION_NOT_DEFINED;
#endif
}


const char*
ls_info_git_commit() {
#if (defined(GIT_COMMIT))
	return GIT_COMMIT;
#else
	return OPTION_NOT_DEFINED;
#endif
}
