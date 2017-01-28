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


#include "./detect_platform.h"


#define LS_COMPILER_ID_UNKNOWN				0
#define LS_COMPILER_ID_GCC					1
#define LS_COMPILER_ID_MSC					2
#define LS_COMPILER_ID_INTEL				3
#define LS_COMPILER_ID_LLVM					4

#if (!defined(LS_COMPILER))
#	if (defined(_MSC_VER))
#		define LS_COMPILER					LS_COMPILER_ID_MSC
#		define LS_COMPILER_STRING			"MSC"
#		define LS_COMPILER_STRING_FULL		"Microsoft Visual C++"
#		define LS_ATTR_THREAD			__declspec(thread)
#	elif (defined(__llvm__) || defined(__clang__))
#		define LS_COMPILER					LS_COMPILER_ID_LLVM
#		define LS_COMPILER_STRING			"LLVM"
#		define LS_COMPILER_STRING_FULL		"Low Level Virtual Machine"
#		define LS_ATTR_THREAD				__thread
#		define LS_ATTR_USED					__attribute__((used))
#	elif (defined(__GNUC__))
#		define LS_COMPILER					LS_COMPILER_ID_GCC
#		define LS_COMPILER_STRING			"GCC"
#		define LS_COMPILER_STRING_FULL		"GNU Compiler Collection"
#		define LS_ATTR_THREAD				__thread
#		define LS_ATTR_USED					__attribute__((used))
#	elif (defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC))
#		define LS_COMPILER					LS_COMPILER_ID_INTEL
#		define LS_COMPILER_STRING			"ICL"
#		define LS_COMPILER_STRING_FULL		"Intel C/C++"
#		if (LS_WINDOWS)
#			define LS_ATTR_THREAD			__declspec(thread)
#		else
#			define LS_ATTR_THREAD			__thread
#		endif
#	endif
#endif

#ifndef LS_ATTR_THREAD
#define LS_ATTR_THREAD
#endif

#ifndef LS_ATTR_USED
#define LS_ATTR_USED
#endif


#endif