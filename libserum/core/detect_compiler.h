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

#ifndef __LS_CORE_DETECT_COMPILER_H
#define __LS_CORE_DETECT_COMPILER_H


#include "./detect_os.h"


#define LS_COMPILER_ID_UNKNOWN				0
#define LS_COMPILER_ID_GCC					1
#define LS_COMPILER_ID_MSC					2
#define LS_COMPILER_ID_INTEL				3
#define LS_COMPILER_ID_LLVM					4


#if (MAKEFILE)
#	define LS_MAKEFILE_STRING				"Makefile"
#else
#	define LS_MAKEFILE_STRING				"Custom build system"
#endif


// Determine what compiler we're dealing with.
#if (!defined(LS_COMPILER))
#	if (defined(_MSC_VER))
#		define LS_MSC						1
#	elif (defined(__llvm__) || defined(__clang__))
#		define LS_LLVM						1
#	elif (defined(__GNUC__))
#		define LS_GCC						1
#	elif (defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC))
#		define LS_ICL						1
#	endif
#endif


// Make sure all compiler definitions are defined.
#ifndef LS_MSC
#define LS_MSC								0
#endif

#ifndef LS_LLVM
#define LS_LLVM								0
#endif

#ifndef LS_GCC
#define LS_GCC								0
#endif

#ifndef LS_ICL
#define LS_ICL								0
#endif


// Set compiler-specific definitions.
#if (LS_MSC)
#	define LS_COMPILER						LS_COMPILER_ID_MSC
#
#	define LS_COMPILER_STRING				"MSC"
#	define LS_COMPILER_STRING_FULL			"Microsoft Visual C++"
#	define LS_COMPILER_VERSION				MACRO_STRINGIFY(_MSC_VER)
#
#	define LS_ATTR_THREAD					__declspec(thread)
#	define LS_ATTR_INLINE					inline
#	define LS_ATTR_ALIGN(x)					__declspec(align(x))
#
#	define LS_RESTRICT						__restrict
#
#	define LS_COMPILER_MESG(msg)			__pragma(message(msg))
#
#	define NO_MIN_MAX
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>

	typedef intptr_t ssize_t;
#elif (LS_LLVM || LS_GCC)
#	define LS_COMPILER						LS_COMPILER_ID_GCC
#
#	define LS_COMPILER_STRING				"GCC"
#	define LS_COMPILER_STRING_FULL			"GNU Compiler Collection"
#	define LS_COMPILER_VERSION				__VERSION__
#
#	define LS_ATTR_THREAD					__thread
#	define LS_ATTR_USED						__attribute__((used))
#	define LS_ATTR_CONSTRUCTOR				__attribute__((constructor))
#	define LS_ATTR_DESTRUCTOR				__attribute__((destructor))
#	define LS_ATTR_INLINE					inline
#	define LS_ATTR_ALIGN(x)					__attribute__((alignment(x)))
#
#	define LS_RESTRICT						restrict
#
#	define LS_COMPILER_MESG(msg)			_Pragma(MACRO_STRINGIFY(message msg))
#elif (LS_ICL)
#	define LS_COMPILER						LS_COMPILER_ID_INTEL
#
#	define LS_COMPILER_STRING				"ICL"
#	define LS_COMPILER_STRING_FULL			"Intel C/C++"
#
#	if (LS_WINDOWS)
#		define LS_ATTR_THREAD				__declspec(thread)
#	else
#		define LS_ATTR_THREAD				__thread
#	endif
#endif


#define LS_COMPILER_FEATURE_MISSING			"Compiler feature missing"

#if (!defined(LS_COMPILER_MESG))
#	if (!defined(DONT_WANT_COMPILER_LOGGING))
#		error Compiler logging unavailable. To ignore this warning, use: -DDONT_WANT_COMPILER_LOGGING
#	endif
#	define LS_COMPILER_MESG(msg)
#endif
#define LS_COMPILER_INFO_PREFIX				"info"
#define LS_COMPILER_WARN_PREFIX				"warn"
#define LS_COMPILER_INFO(msg)				LS_COMPILER_MESG("  " LS_COMPILER_INFO_PREFIX ": " msg)
#define LS_COMPILER_WARN(msg)				LS_COMPILER_MESG("  " LS_COMPILER_WARN_PREFIX ": " msg)
#define LS_COMPILER_WARN_FEATURE(desc)		LS_COMPILER_WARN(LS_COMPILER_FEATURE_MISSING ": " desc)


#if (!defined(LS_COMPILER_VERSION))
#	define LS_COMPILER_VERSION				"v/unknown"
#endif

#if (!defined(LS_ATTR_THREAD))
#	define LS_ATTR_THREAD
#	define LS_NO_ATTR_THREAD
	LS_COMPILER_WARN_FEATURE("attribute for thread-local storage");
#endif

#if (!defined(LS_ATTR_USED))
#	define LS_ATTR_USED
#	define LS_NO_ATTR_USED
	LS_COMPILER_WARN_FEATURE("attribute for marking a variable or function as used");
#endif

#if (!defined(LS_ATTR_CONSTRUCTOR))
#	define LS_ATTR_CONSTRUCTOR
#	define LS_NO_ATTR_CONSTRUCTOR
	LS_COMPILER_WARN_FEATURE("attribute for marking a function as constructor");
#endif

#if (!defined(LS_ATTR_DESTRUCTOR))
#	define LS_ATTR_DESTRUCTOR
#	define LS_NO_ATTR_DESTRUCTOR
	LS_COMPILER_WARN_FEATURE("attribute for marking a function as destructor");
#endif

#if (!defined(LS_ATTR_INLINE))
#	define LS_ATTR_INLINE
#	define LS_NO_ATTR_INLINE
	LS_COMPILER_WARN_FEATURE("attribute for function inlining");
#endif


#if (!defined(LS_RESTRICT))
#	define LS_RESTRICT
#	define LS_NO_RESTRICT
	LS_COMPILER_WARN_FEATURE("keyword for restricted pointers");
#endif


#include "./macro.h"


#endif
