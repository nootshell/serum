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


#ifndef __LS_CORE_SETUP_BITOPS_H
#define __LS_CORE_SETUP_BITOPS_H




#include "./intrinsics.h"
#include "./bitops-inline.h"




#define LS_FLAG(flags, flag)				(((flags) & (flag)) == (flag))




#if (!LS_SANITIZE_BYTES)
static const uint8_t __ls_sanitize_m__ = ((1U << LS_BITS_BYTE) - 1);
#	define LS_BYTE_SANITIZE(x)				((x) & __ls_sanitize_m__)
#else
#	define LS_BYTE_SANITIZE(x)				x
#endif




#if (!LS_INTRINSICS_GOT_ROTL)
#	if (!LS_INTRINSICS_IGNORE_ROTL)
		LS_COMPILER_LOG("Missing ROTL intrinsics, using fallback.")
#	endif

#	define LS_ROTL8							__ls_rotl8
#	define LS_ROTL16						__ls_rotl16
#	define LS_ROTL32						__ls_rotl32
#	define LS_ROTL64						__ls_rotl64
#endif

#if (!LS_INTRINSICS_GOT_ROTR)
#	if (!LS_INTRINSICS_IGNORE_ROTR)
		LS_COMPILER_LOG("Missing ROTR intrinsics, using fallback.")
#	endif

#	define LS_ROTR8							__ls_rotr8
#	define LS_ROTR16						__ls_rotr16
#	define LS_ROTR32						__ls_rotr32
#	define LS_ROTR64						__ls_rotr64
#endif





#if (!LS_INTRINSICS_GOT_SWAP)
#	define LS_SWAP_16(x) (					 \
		(((x) & 0xFF00            ) >>  8) | \
		(((x) & 0x00FF            ) <<  8)	 \
	)
#	define LS_SWAP_32(x) (					 \
		(((x) & 0xFF000000        ) >> 24) | \
		(((x) & 0x00FF0000        ) >>  8) | \
		(((x) & 0x0000FF00        ) <<  8) | \
		(((x) & 0x000000FF        ) << 24)	 \
	)
#	define LS_SWAP_64(x) (					 \
		(((x) & 0xFF00000000000000) >> 56) | \
		(((x) & 0x00FF000000000000) >> 40) | \
		(((x) & 0x0000FF0000000000) >> 24) | \
		(((x) & 0x000000FF00000000) >>  8) | \
		(((x) & 0x00000000FF000000) <<  8) | \
		(((x) & 0x0000000000FF0000) << 24) | \
		(((x) & 0x000000000000FF00) << 40) | \
		(((x) & 0x00000000000000FF) << 56)	 \
	)
#endif


#if (LS_BIG_ENDIAN)
#	define LS_ENSURE_BIG16(x)				(x)
#	define LS_ENSURE_BIG32(x)				(x)
#	define LS_ENSURE_BIG64(x)				(x)
#	define LS_ENSURE_LITTLE16(x)			LS_SWAP_16((x))
#	define LS_ENSURE_LITTLE32(x)			LS_SWAP_32((x))
#	define LS_ENSURE_LITTLE64(x)			LS_SWAP_64((x))
#else
#	define LS_ENSURE_BIG16(x)				LS_SWAP_16((x))
#	define LS_ENSURE_BIG32(x)				LS_SWAP_32((x))
#	define LS_ENSURE_BIG64(x)				LS_SWAP_64((x))
#	define LS_ENSURE_LITTLE16(x)			(x)
#	define LS_ENSURE_LITTLE32(x)			(x)
#	define LS_ENSURE_LITTLE64(x)			(x)
#endif




#endif
