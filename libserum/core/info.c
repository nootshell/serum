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


ID("compile-time build information");


const char*
ls_info_compilation_environment() {
	return LS_COMPILATION_ENVIRONMENT;
}


const char*
ls_info_compilation_options() {
#define OPTION_NOT_DEFINED					"[not defined]"
	return

		"DEBUG                           "
#if (defined(DEBUG))
		MACRO_STRINGIFY(DEBUG)
#else
		OPTION_NOT_DEFINED
#endif

		"\nDISTRO                          "
#if (defined(DISTRO))
		MACRO_STRINGIFY(DISTRO)
#else
		OPTION_NOT_DEFINED
#endif

		"\nLIBSERUM_EXPORTS                "
#if (defined(LIBSERUM_EXPORTS))
		MACRO_STRINGIFY(LIBSERUM_EXPORTS)
#else
		OPTION_NOT_DEFINED
#endif

		"\nLS_ISO9797_ALLOW_ALL_ZERO       "
#if (defined(LS_ISO9797_ALLOW_ALL_ZERO))
		MACRO_STRINGIFY(LS_ISO9797_ALLOW_ALL_ZERO)
#else
		OPTION_NOT_DEFINED
#endif

		"\nLS_ISO9797_DENY_SIZE_ZERO       "
#if (defined(LS_ISO9797_DENY_SIZE_ZERO))
		MACRO_STRINGIFY(LS_ISO9797_DENY_SIZE_ZERO)
#else
		OPTION_NOT_DEFINED
#endif

		"\nLS_LOG_ORIGIN                   "
#if (defined(LS_LOG_ORIGIN))
		MACRO_STRINGIFY(LS_LOG_ORIGIN)
#else
		OPTION_NOT_DEFINED
#endif

		"\nLS_LOG_RESULTS                  "
#if (defined(LS_LOG_RESULTS))
		MACRO_STRINGIFY(LS_LOG_RESULTS)
#else
		OPTION_NOT_DEFINED
#endif

		"\nLS_MD5_DENY_SIZE_ZERO           "
#if (defined(LS_MD5_DENY_SIZE_ZERO))
		MACRO_STRINGIFY(LS_MD5_DENY_SIZE_ZERO)
#else
		OPTION_NOT_DEFINED
#endif

		"\nLS_RDTSC_NANOS_FAILSAFE         "
#if (defined(LS_RDTSC_NANOS_FAILSAFE))
		MACRO_STRINGIFY(LS_RDTSC_NANOS_FAILSAFE)
#else
		OPTION_NOT_DEFINED
#endif

		"\nLS_RIJNDAEL_STRICT              "
#if (defined(LS_RIJNDAEL_STRICT))
		MACRO_STRINGIFY(LS_RIJNDAEL_STRICT)
#else
		OPTION_NOT_DEFINED
#endif

		"\nLS_SELFTEST_CRYPTO_HASHING      "
#if (defined(LS_SELFTEST_CRYPTO_HASHING))
		MACRO_STRINGIFY(LS_SELFTEST_CRYPTO_HASHING)
#else
		OPTION_NOT_DEFINED
#endif

		"\nLS_SELFTEST_VERBOSE             "
#if (defined(LS_SELFTEST_VERBOSE))
		MACRO_STRINGIFY(LS_SELFTEST_VERBOSE)
#else
		OPTION_NOT_DEFINED
#endif

		"\nLS_SHA2_DENY_SIZE_ZERO          "
#if (defined(LS_SHA2_DENY_SIZE_ZERO))
		MACRO_STRINGIFY(LS_SHA2_DENY_SIZE_ZERO)
#else
		OPTION_NOT_DEFINED
#endif

		// Use compiler intrinsics to do byte swaps?
		"\nLS_SWAP_USE_INTRINSICS          "
#if (defined(LS_SWAP_USE_INTRINSICS))
		MACRO_STRINGIFY(LS_SWAP_USE_INTRINSICS)
#else
		OPTION_NOT_DEFINED
#endif

		"\nMAKEFILE                        "
#if (defined(MAKEFILE))
		MACRO_STRINGIFY(MAKEFILE)
#else
		OPTION_NOT_DEFINED
#endif

		"\nRELEASE                         "
#if (defined(RELEASE))
		MACRO_STRINGIFY(RELEASE)
#else
		OPTION_NOT_DEFINED
#endif

		;
}
