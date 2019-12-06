#ifndef __SERUM_CORE_PLATFORM_DETECTION_H
#define __SERUM_CORE_PLATFORM_DETECTION_H

#ifndef __SERUM_CORE_PLATFORM_H
#	error Expecting to be included from serum/core/platform.h, which was not detected.
#endif




#define SERUM_ID_UNKNOWN					0

#define SERUM_ID_LINUX						0x11
#define SERUM_ID_ANDROID					0x12
#define SERUM_ID_MAC						0x13
#define SERUM_ID_WINDOWS					0xDEFEC8ED

#define SERUM_ID_GCC						0x21
#define SERUM_ID_MSC						0x22
#define SERUM_ID_LLVM						0x23

#define SERUM_ID_ENDIAN_LITTLE				0x31
#define SERUM_ID_ENDIAN_BIG					0x32




/* Determine operating system. */
#if (!SERUM_ID_FOUND_OS)
#	if (defined(__gnu_linux__) || defined(__linux__) || defined(linux))
#		define SERUM_LINUX					1
#		if (defined(__ANDROID__) || defined(__ANDROID_API__))
#			define SERUM_ANDROID			1
#			define SERUM_ID_FOUND_OS		SERUM_ID_ANDROID
#			define SERUM_OS_STRING			"Linux (Android)"
#		else
#			define SERUM_ID_FOUND_OS		SERUM_ID_LINUX
#			define SERUM_OS_STRING			"Linux"
#		endif
#	elif (defined(_WIN32))
#		define SERUM_WINDOWS				1
#		define SERUM_ID_FOUND_OS			SERUM_ID_WINDOWS
#		define SERUM_OS_STRING				"Windows"
#	elif (defined(__APPLE__) && defined(__MACH__))
#		define SERUM_MAC					1
#		define SERUM_ID_FOUND_OS			SERUM_ID_MAC
#		define SERUM_OS_STRING				"Mac"
#	else
#		define SERUM_NO_OS					1
#		define SERUM_ID_FOUND_OS			SERUM_ID_UNKNOWN
#		define SERUM_OS_STRING				"Unknown"
#	endif
#endif


/* Determine compiler. */
#if (!SERUM_ID_FOUND_COMPILER)
#	if (defined(_MSC_VER))
#		define SERUM_ID_FOUND_COMPILER		SERUM_ID_MSC
#		define SERUM_MSC					1
#		define SERUM_COMPILER_STRING		"MSC"
#		define SERUM_COMPILER_VERSION		SERUM_STRINGIFY(_MSC_VER)
#	elif (defined(__llvm__) || defined(__clang__))
#		define SERUM_ID_FOUND_COMPILER		SERUM_ID_LLVM
#		define SERUM_LLVM					1
#		define SERUM_COMPILER_STRING		"LLVM"
#		define SERUM_COMPILER_VERSION		__VERSION__
#	elif (defined(__GNUC__))
#		define SERUM_ID_FOUND_COMPILER		SERUM_ID_GCC
#		define SERUM_GCC					1
#		define SERUM_COMPILER_STRING		"GCC"
#		define SERUM_COMPILER_VERSION		__VERSION__
#	else
#		define SERUM_NO_COMPILER			1
#		define SERUM_ID_FOUND_COMPILER		SERUM_ID_UNKNOWN
#		define SERUM_COMPILER_STRING		"Unknown"
#	endif
#endif

/* Ensure SERUM_COMPILER_VERSION has a value. */
#ifndef SERUM_COMPILER_VERSION
#	define SERUM_COMPILER_VERSION			"unknown"
#endif


/* Determine endianness. */
#if (!defined(SERUM_ID_FOUND_ENDIAN))
#	if (defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIPSEB) || defined(__MIPSEB) || defined(__MIPSEB__) || ((SERUM_WINDOWS && REG_DWORD != REG_DWORD_LITTLE_ENDIAN) || (!SERUM_WINDOWS && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__))))
#		define SERUM_ENDIAN_BIG				1
#		define SERUM_ID_FOUND_ENDIAN		SERUM_ID_ENDIAN_BIG
#		define SERUM_ENDIAN_STRING			"big"
#	else
#		define SERUM_ENDIAN_LITTLE			1
#		define SERUM_ID_FOUND_ENDIAN		SERUM_ID_ENDIAN_LITTLE
#		define SERUM_ENDIAN_STRING			"little"
#	endif
#endif




/* Check if the compiler has a certain builtin available. */
#ifdef __has_builtin
#	define SPF_BUILTIN_AVAIL(x)				__has_builtin(x)
#endif

/* Check if the given include file is available. */
#ifdef __has_include
#	define SPF_INCLUDE_AVAIL(x)				__has_include(<x>)
#endif




#endif
