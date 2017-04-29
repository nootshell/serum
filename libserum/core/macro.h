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

#ifndef __LS_CORE_MACRO_H
#define __LS_CORE_MACRO_H


#define _MACRO_STRINGIFY(x)					#x
#define MACRO_STRINGIFY(x)					_MACRO_STRINGIFY(x)


#if (LS_USE_CHECK_BUILTIN)
#	if (defined(__has_builtin))
#		define LS_CHECK_BUILTIN(x)			__has_builtin(x)
#		define LS_HAVE_CHECK_BUILTIN		1
#	else
#		if (defined(LS_COMPILER_WARN_FEATURE))
			LS_COMPILER_WARN_FEATURE("macro definition to check for builtins");
#		endif
#	endif
#else
#	define LS_CHECK_BUILTIN(x)				0
#endif

#if (LS_USE_CHECK_INCLUDE)
#	if (defined(__has_include))
#		define LS_CHECK_INCLUDE(x)			__has_include(<x>)
#		define LS_HAVE_CHECK_INCLUDE		1
#	else
#		if (defined(LS_COMPILER_WARN_FEATURE))
			LS_COMPILER_WARN_FEATURE("macro definition to check for includes");
#		endif
#	endif
#else
#	if (LS_NO_CHECK_INCLUDE_ASSUME_EXISTS)
#		define LS_CHECK_INCLUDE(x)			1
#	else
#		define LS_CHECK_INCLUDE(x)			0
#	endif
#endif


#if (!defined(LS_HAVE_CHECK_BUILTIN))
#	define LS_HAVE_CHECK_BUILTIN			0
#endif

#if (!defined(LS_HAVE_CHECK_INCLUDE))
#	define LS_HAVE_CHECK_INCLUDE			0
#endif


#endif
