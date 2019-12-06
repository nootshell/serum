/*
**	FIXME: For (the glorious) Windows platform, endianness will not be detected properly if REG_DWORD and REG_DWORD_LITTLE_ENDIAN are not defined (by e.g. inclusion of Windows.h).
*/

#ifndef __SERUM_CORE_PLATFORM_H
#define __SERUM_CORE_PLATFORM_H




#include "./macro.h"




#include "./platform-detection.h"
#include "./platform-setup.h"
#include "./platform-logging.h"

#if (!defined(SPF_INCLUDE_AVAIL))
#	if (!defined(SPF_INCLUDE_AVAIL_DEFAULT))
#		define SPF_INCLUDE_AVAIL_DEFAULT	1
#	endif
#	define SPF_INCLUDE_AVAIL(x)				SPF_INCLUDE_AVAIL_DEFAULT
#	define SERUM_NO_INCLUDE_AVAIL			1
	SERUM_COMPILER_WARN_FEATURE("macro to check for include file availability");
#endif

#if (!defined(SPF_BUILTIN_AVAIL))
#	if (!defined(SPF_BUILTIN_AVAIL_DEFAULT))
#		define SPF_BUILTIN_AVAIL_DEFAULT	1
#	endif
#	define SPF_BUILTIN_AVAIL(x)				SPF_BUILTIN_AVAIL_DEFAULT
#	define SERUM_NO_BUILTIN_AVAIL			1
	SERUM_COMPILER_WARN_FEATURE("macro to check for builtin availability");
#endif

#if (!defined(SATTR_THREADLOCAL))
#	define SATTR_THREADLOCAL
#	define SERUM_NO_ATTR_THREAD				1
	SERUM_COMPILER_WARN_FEATURE("attribute for thread-local storage");
#endif

#if (!defined(SATTR_USED))
#	define SATTR_USED
#	define SERUM_NO_ATTR_USED				1
	SERUM_COMPILER_WARN_FEATURE("attribute for marking a variable or function as used");
#endif

#if (!defined(SATTR_CONSTRUCTOR))
#	define SATTR_CONSTRUCTOR
#	define SERUM_NO_ATTR_CONSTRUCTOR		1
	SERUM_COMPILER_WARN_FEATURE("attribute for marking a function as constructor");
#endif

#if (!defined(SATTR_DESTRUCTOR))
#	define SATTR_DESTRUCTOR
#	define SERUM_NO_ATTR_DESTRUCTOR			1
	SERUM_COMPILER_WARN_FEATURE("attribute for marking a function as destructor");
#endif

#if (!defined(SATTR_INLINE))
#	define SATTR_INLINE
#	define SERUM_NO_ATTR_INLINE				1
	SERUM_COMPILER_WARN_FEATURE("attribute for function inlining");
#endif

#if (!defined(SATTR_NONNULL))
#	define SATTR_NONNULL(params)
#	define SERUM_NO_ATTR_NONNULL			1
	SERUM_COMPILER_WARN_FEATURE("attribute for non-null pointers");
#endif

#if (!defined(SATTR_RESTRICT))
#	define SATTR_RESTRICT
#	define SERUM_NO_RESTRICT				1
	SERUM_COMPILER_WARN_FEATURE("keyword for restricted pointers");
#endif




#include "./platform-bits.h"




#endif
