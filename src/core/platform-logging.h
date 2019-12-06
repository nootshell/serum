#ifndef __SERUM_CORE_PLATFORM_LOGGING_H
#define __SERUM_CORE_PLATFORM_LOGGING_H

#ifndef __SERUM_CORE_PLATFORM_H
#	error Expecting to be included from serum/core/platform.h, which was not detected.
#endif




/* Setup compiler-based build logging. */
#if (SERUM_BUILD_MUTE)
#	undef SERUM_COMPILER_MESG
#	define SERUM_COMPILER_MESG(msg)
#elif (!defined(SERUM_COMPILER_MESG))
#	error Compiler logging unavailable.
#	define SERUM_COMPILER_MESG(msg)
#endif

#define SERUM_COMPILER_INFO_PREFIX			"info"
#define SERUM_COMPILER_WARN_PREFIX			"warn"

#define SERUM_COMPILER_FEATURE_MISSING		"Compiler feature missing"

#define SERUM_COMPILER_INFO(msg)			SERUM_COMPILER_MESG("  " SERUM_COMPILER_INFO_PREFIX ": " msg)
#define SERUM_COMPILER_WARN(msg)			SERUM_COMPILER_MESG("  " SERUM_COMPILER_WARN_PREFIX ": " msg)
#define SERUM_COMPILER_WARN_FEATURE(msg)	SERUM_COMPILER_WARN(SERUM_COMPILER_FEATURE_MISSING ": " msg)




#endif
