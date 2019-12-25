#ifndef __SERUM_CORE_PLATFORM_SETUP_H
#define __SERUM_CORE_PLATFORM_SETUP_H

#ifndef __SERUM_CORE_PLATFORM_H
#	error Expecting to be included from serum/core/platform.h, which was not detected.
#endif




/* Compiler-specific setup. */
#if (SERUM_MSC)
#	define SATTR(x)							__declspec(x)
#	define SATTR_THREADLOCAL				SATTR(thread)
#	define SATTR_INLINE						inline
#	define SATTR_ALIGN(x)					SATTR(align(x))
#	define SATTR_RESTRICT					__restrict

#	define SERUM_COMPILER_MESG(msg)			__pragma(message(msg))
//#	define SERUM_COMPILER_PUSH				__pragma()
//#	define SERUM_COMPILER_POP				__pragma()
//#	define SERUM_COMPILER_PEDANTIC			__pragma()

#	define NO_MIN_MAX	/* Ensure the glorious Windows SDK doesn't reserve 'min' and 'max'. */
#	define WIN32_LEAN_AND_MEAN
#elif (SERUM_LLVM || SERUM_GCC)
#	define SATTR(x)							__attribute__((x))
#	define SATTR_THREADLOCAL				__thread
#	define SATTR_USED						SATTR(used)
#	define SATTR_CONSTRUCTOR				SATTR(constructor)
#	define SATTR_DESTRUCTOR					SATTR(destructor)
#	define SATTR_INLINE						inline
#	define SATTR_ALIGN(x)					SATTR(alignment(x))
#	define SATTR_NONNULL(params)			SATTR(__nonnull__ params)
#	define SATTR_NONNULL_EX(...)			SATTR_NONNULL((__VA_ARGS__))
#	define SATTR_PURE						SATTR(pure)
#	define SATTR_CONST						SATTR(const)
#	define SATTR_RESTRICT					__restrict__

#	define SERUM_COMPILER_MESG(msg)			_Pragma(SERUM_STRINGIFY(message msg))
#	define SERUM_COMPILER_PUSH				_Pragma("GCC diagnostic push")
#	define SERUM_COMPILER_POP				_Pragma("GCC diagnostic pop")
#	define SERUM_COMPILER_PEDANTIC			_Pragma("GCC diagnostic error \"-Wpedantic\"")
#endif




#endif
