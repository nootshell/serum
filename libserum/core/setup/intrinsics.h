/******************************************************************************
**                                                                           **
**   The MIT License                                                         **
**                                                                           **
**   Copyright 2017-2018 icecubetray                                         **
**                                                                           **
**   Permission is hereby granted, free of charge, to any person             **
**   obtaining a copy of this software and associated documentation files    **
**   (the "Software"), to deal in the Software without restriction,          **
**   including without limitation the rights to use, copy, modify, merge,    **
**   publish, distribute, sublicense, and/or sell copies of the Software,    **
**   and to permit persons to whom the Software is furnished to do so,       **
**   subject to the following conditions:                                    **
**                                                                           **
**   The above copyright notice and this permission notice shall be          **
**   included in all copies or substantial portions of the Software.         **
**                                                                           **
**   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,         **
**   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF      **
**   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  **
**   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    **
**   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    **
**   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       **
**   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  **
**                                                                           **
******************************************************************************/



#ifndef __LS_CORE_SETUP_INTRINSICS_H
#define __LS_CORE_SETUP_INTRINSICS_H



#include "./platform-detection.h"



#if (LS_GCC || LS_LLVM)
#	include <x86intrin.h>
#	define LS_INTRINSICS					1
#elif (LS_MSC || LS_MINGW)
#	include <intrin.h>
#	define LS_INTRINSICS					1
#endif



#undef LS_INTRINSICS_GOT_ROTL
#undef LS_INTRINSICS_GOT_ROTR
#undef LS_INTRINSICS_GOT_RDTSC

#if (LS_INTRINSICS)
#	if (LS_MSC || LS_MINGW)
#		if ((LS_ID_ARCH >= 1 && LS_ID_ARCH <= 3) && LS_UCRT)
#			define LS_ROTL8					_rotl8
#			define LS_ROTL16				_rotl16
#			define LS_ROTL32				_rotl
#			define LS_ROTL64				_rotl64
#			define LS_INTRINSICS_GOT_ROTL	1

#			define LS_ROTR8					_rotr8
#			define LS_ROTR16				_rotr16
#			define LS_ROTR32				_rotr
#			define LS_ROTR64				_rotr64
#			define LS_INTRINSICS_GOT_ROTR	1
#
#			define LS_RDTSC					__rdtsc
#			define LS_INTRINSICS_GOT_RDTSC	1
#		endif
#	elif (LS_GCC || LS_LLVM)
		// GCC and LLVM should be optimizing our fallback implementations nicely.
#		define LS_INTRINSICS_IGNORE_ROTL	1
#		define LS_INTRINSICS_IGNORE_ROTR	1
#	endif
#endif



#endif
