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




/*! \hideinitializer \brief Used to determine if \p flag's bits are set in \p flags. */
#define LS_FLAG(flags, flag)				(((flags) & (flag)) == (flag))




#if (!LS_INTRINSICS_GOT_ROTL || LS_DOXYGEN)
#	if (!LS_INTRINSICS_IGNORE_ROTL)
		LS_COMPILER_LOG("Missing ROTL intrinsics, using fallback.")
#	endif
#
#	/*! \hideinitializer \brief Alias to compiler builtin or \link __ls_rotl8 fallback\endlink for: rotate left, 8-bit */
#	define LS_ROTL8							__ls_rotl8
#
#	/*! \hideinitializer \brief Alias to compiler builtin or \link __ls_rotl16 fallback\endlink for: rotate left, 16-bit */
#	define LS_ROTL16						__ls_rotl16
#
#	/*! \hideinitializer \brief Alias to compiler builtin or \link __ls_rotl32 fallback\endlink for: rotate left, 32-bit */
#	define LS_ROTL32						__ls_rotl32
#
#	/*! \hideinitializer \brief Alias to compiler builtin or \link __ls_rotl64 fallback\endlink for: rotate left, 64-bit */
#	define LS_ROTL64						__ls_rotl64
#endif


#if (!LS_INTRINSICS_GOT_ROTR || LS_DOXYGEN)
#	if (!LS_INTRINSICS_IGNORE_ROTR)
		LS_COMPILER_LOG("Missing ROTR intrinsics, using fallback.")
#	endif
#
#	/*! \hideinitializer \brief Alias to compiler builtin or \link __ls_rotr8 fallback\endlink for: rotate right, 8-bit */
#	define LS_ROTR8							__ls_rotr8
#
#	/*! \hideinitializer \brief Alias to compiler builtin or \link __ls_rotr16 fallback\endlink for: rotate right, 16-bit */
#	define LS_ROTR16						__ls_rotr16
#
#	/*! \hideinitializer \brief Alias to compiler builtin or \link __ls_rotr32 fallback\endlink for: rotate right, 32-bit */
#	define LS_ROTR32						__ls_rotr32
#
#	/*! \hideinitializer \brief Alias to compiler builtin or \link __ls_rotr64 fallback\endlink for: rotate right, 64-bit */
#	define LS_ROTR64						__ls_rotr64
#endif





#if (!LS_INTRINSICS_GOT_SWAP || LS_DOXYGEN)
#	/*! \hideinitializer \brief Returns the endianness-swapped value of the given 16-bit integer, either by compiler builtin or bitwise fallback. */
#	define LS_SWAP_16(x) (					 \
		(((x) & 0xFF00            ) >>  8) | \
		(((x) & 0x00FF            ) <<  8)	 \
	)
#
#	/*! \hideinitializer \brief Returns the endianness-swapped value of the given 32-bit integer, either by compiler builtin or bitwise fallback. */
#	define LS_SWAP_32(x) (					 \
		(((x) & 0xFF000000        ) >> 24) | \
		(((x) & 0x00FF0000        ) >>  8) | \
		(((x) & 0x0000FF00        ) <<  8) | \
		(((x) & 0x000000FF        ) << 24)	 \
	)
#
#	/*! \hideinitializer \brief Returns the endianness-swapped value of the given 64-bit integer, either by compiler builtin or bitwise fallback. */
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


#if (LS_BIG_ENDIAN || LS_DOXYGEN)
#	/*! \hideinitializer \brief Returns the big-endian value of the given \em native 16-bit integer. */
#	define LS_ENSURE_BIG16(x)				(x)
#
#	/*! \hideinitializer \brief Returns the big-endian value of the given \em native 32-bit integer. */
#	define LS_ENSURE_BIG32(x)				(x)
#
#	/*! \hideinitializer \brief Returns the big-endian value of the given \em native 64-bit integer. */
#	define LS_ENSURE_BIG64(x)				(x)
#
#	/*! \hideinitializer \brief Returns the little-endian value of the given \em native 16-bit integer. */
#	define LS_ENSURE_LITTLE16(x)			LS_SWAP_16((x))
#
#	/*! \hideinitializer \brief Returns the little-endian value of the given \em native 32-bit integer. */
#	define LS_ENSURE_LITTLE32(x)			LS_SWAP_32((x))
#
#	/*! \hideinitializer \brief Returns the little-endian value of the given \em native 64-bit integer. */
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
