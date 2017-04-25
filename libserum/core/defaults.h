#ifndef __LS_CORE_DEFAULTS_H
#define __LS_CORE_DEFAULTS_H


// Defaultless: MAKEFILE, DISTRO, GIT_BRANCH, GIT_COMMIT
//              ELF_INTERP, LS_LOG_RESULTS,
//              LS_NO_CHECK_INCLUDE_ASSUME_EXISTS


#if (!defined(LS_ANSI_ESCAPE_SUPPORT))
#	include "./detect_os.h"
#	if (LS_WINDOWS)
#		define LS_ANSI_ESCAPE_SUPPORT		0
#	else
#		define LS_ANSI_ESCAPE_SUPPORT		1
#	endif
#endif

#if (!defined(LS_LOG_ORIGIN))
#	if (DEBUG)
#		define LS_LOG_ORIGIN				1
#	else
#		define LS_LOG_ORIGIN				0
#	endif
#endif

#if (!defined(LS_RDTSC_NANOS_FAILSAFE))
#	define LS_RDTSC_NANOS_FAILSAFE			0
#endif

#if (!defined(LS_SWAP_USE_INTRINSICS))
#	define LS_SWAP_USE_INTRINSICS			1	// FIXME: May cause slow compilation.
#endif

#if (!defined(LS_SELFTEST))
#	define LS_SELFTEST						1
#endif

#if (!defined(LS_SELFTEST_CRYPTO_HASHING))
#	define LS_SELFTEST_CRYPTO_HASHING		1
#endif

#if (!defined(LS_SELFTEST_STARTUP))
#	define LS_SELFTEST_STARTUP				1
#endif

#if (!defined(LS_ISO9797_ALLOW_ALL_ZERO))
#	define LS_ISO9797_ALLOW_ALL_ZERO		1
#endif

#if (!defined(LS_ISO9797_DENY_SIZE_ZERO))
#	define LS_ISO9797_DENY_SIZE_ZERO		0
#endif

#if (!defined(LS_MD5_DENY_SIZE_ZERO))
#	define LS_MD5_DENY_SIZE_ZERO			0
#endif

#if (!defined(LS_RIJNDAEL_STRICT))
#	define LS_RIJNDAEL_STRICT				1
#endif

#if (!defined(LS_SALSA20_STRICT))
#	define LS_SALSA20_STRICT				1
#endif

#if (!defined(LS_SHA2_DENY_SIZE_ZERO))
#	define LS_SHA2_DENY_SIZE_ZERO			0
#endif

#if (!defined(LS_USE_CHECK_BUILTIN))
#	define LS_USE_CHECK_BUILTIN				1
#endif

#if (!defined(LS_USE_CHECK_INCLUDE))
#	define LS_USE_CHECK_BUILTIN				1
#endif

#if (!defined(LS_VERBOSE_STARTUP))
#	define LS_VERBOSE_STARTUP				0
#endif


#endif
