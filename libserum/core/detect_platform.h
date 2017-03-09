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

#ifndef __LS_CORE_DETECT_PLATFORM_H
#define __LS_CORE_DETECT_PLATFORM_H


#define LS_ARCH_ID_X86						1
#define LS_ARCH_ID_X64						2
#define LS_ARCH_ID_ARM						3

#if (!defined(LS_ARCH))
#	if (defined(__amd64) || defined(__amd64__) || defined(__x86_64) || defined(__x86_64__) || defined(__ppc64__) || defined(_M_X64) || defined(_M_AMD64))
#		define LS_ARCH						LS_ARCH_ID_X64
#		define LS_ARCH_BITS					64
#		define LS_ARCH_STRING				"x64"
#	elif (defined(__arm) || defined(__arm__) || defined(__thumb__) || defined(_ARM) || defined(_M_ARM) || defined(_M_ARMT) || defined(__ARMCC_VERSION) || defined(__CC_ARM))
#		define LS_ARCH						LS_ARCH_ID_ARM
#		if (defined(__ARM_ARCH_2__))
#			define LS_ARCH_ARM_VERSION		2
#		elif (defined(__ARM_ARCH_3__))
#			define LS_ARCH_ARM_VERSION		3
#		elif (defined(__ARM_ARCH_4__))
#			define LS_ARCH_ARM_VERSION		4
#		elif (defined(__ARM_ARCH_5__))
#			define LS_ARCH_ARM_VERSION		5
#		elif (defined(__ARM_ARCH_6__))
#			define LS_ARCH_ARM_VERSION		6
#		elif (defined(__ARM_ARCH_7__))
#			define LS_ARCH_ARM_VERSION		7
#		elif (defined(__ARM_ARCH_8__))
#			define LS_ARCH_ARM_VERSION		8
#		elif (defined(__ARM_ARCH))
#			define LS_ARCH_ARM_VERSION		__ARM_ARCH
#		endif
#		if (defined(LS_ARCH_ARM_VERSION))
#			define LS_ARCH_STRING			"ARMv"MACRO_STRINGIFY(LS_ARCH_ARM_VERSION)
#		else
#			define LS_ARCH_STRING			"ARM"
#		endif
#		if (defined(__aarch64__))
#			define LS_ARCH_BITS				64
#		else
#			define LS_ARCH_BITS				32
#		endif
#	else
#		define LS_ARCH						LS_ARCH_ID_X86
#		define LS_ARCH_BITS					32
#		define LS_ARCH_STRING				"x86"
#	endif
#endif


#endif
