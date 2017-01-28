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

#ifndef __LS_CORE_INTRINSICS_H
#define __LS_CORE_INTRINSICS_H


#if (defined(_exit_))
#	undef _exit_
#endif


#if (!defined(LS_INTRINSICS))
#	if (defined(__has_include))
#		if (__has_include(<x86intrin.h>))
#			include <x86intrin.h>
#			define LS_INTRINSICS			1
#			if (!defined(LS_INTRINSICS_CONTINUE))
#				define _exit_
#			endif
#		endif
#		if (!defined(_exit_) && __has_include(<intrin.h>))
#			include <intrin.h>
#			define LS_INTRINSICS			1
#		endif
#	else
#		include "./detect_compiler.h"
#		if (LS_GCC || LS_LLVM)
#			include <x86intrin.h>
#			define LS_INTRINSICS			1
#		elif (LS_MSC)
#			include <intrin.h>
#			define LS_INTRINSICS			1
#		endif
#	endif
#endif

#if (!defined(LS_INTRINSICS))
#	define LS_INTRINSICS					0
#endif

#if (!defined(LS_INTRINSICS_STRING))
#	if (LS_INTRINSICS)
#		define LS_INTRINSICS_STRING			"+"
#	else
#		define LS_INTRINSICS_STRING			"-"
#	endif
#endif


#if (defined(_exit_))
#	undef _exit_
#endif


#endif
