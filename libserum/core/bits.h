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

#ifndef __LS_CORE_BITS_H
#define __LS_CORE_BITS_H


#define HAS_FLAG(flags, flag)				(((flags) & (flag)) == (flag))


#define LS_ROT(s1, s2, bits, value, shift)	(((value) s1 (shift)) | ((value) s2 (-(shift) & ((bits) - 1))))

#define LS_ROTL(bits, value, shift)			LS_ROT(<<, >>, (bits), (value), (shift))
#define LS_ROTR(bits, value, shift)			LS_ROT(>>, <<, (bits), (value), (shift))

#define LS_ROTL8(value, shift)				LS_ROTL(8, (value), (shift))
#define LS_ROTL16(value, shift)				LS_ROTL(16, (value), (shift))
#define LS_ROTL32(value, shift)				LS_ROTL(32, (value), (shift))
#define LS_ROTL64(value, shift)				LS_ROTL(64, (value), (shift))

#define LS_ROTR8(value, shift)				LS_ROTR(8, (value), (shift))
#define LS_ROTR16(value, shift)				LS_ROTR(16, (value), (shift))
#define LS_ROTR32(value, shift)				LS_ROTR(32, (value), (shift))
#define LS_ROTR64(value, shift)				LS_ROTR(64, (value), (shift))


#define BITMASK(bits)						((1LLU << (bits)) - 1)


#define MKBIT(x)							(1LLU << ((x) - 1))

#define BIT_1								MKBIT( 1)
#define BIT_2								MKBIT( 2)
#define BIT_3								MKBIT( 3)
#define BIT_4								MKBIT( 4)
#define BIT_5								MKBIT( 5)
#define BIT_6								MKBIT( 6)
#define BIT_7								MKBIT( 7)
#define BIT_8								MKBIT( 8)
#define BIT_9								MKBIT( 9)
#define BIT_10								MKBIT(10)
#define BIT_11								MKBIT(11)
#define BIT_12								MKBIT(12)
#define BIT_13								MKBIT(13)
#define BIT_14								MKBIT(14)
#define BIT_15								MKBIT(15)
#define BIT_16								MKBIT(16)
#define BIT_17								MKBIT(17)
#define BIT_18								MKBIT(18)
#define BIT_19								MKBIT(19)
#define BIT_20								MKBIT(20)
#define BIT_21								MKBIT(21)
#define BIT_22								MKBIT(22)
#define BIT_23								MKBIT(23)
#define BIT_24								MKBIT(24)
#define BIT_25								MKBIT(25)
#define BIT_26								MKBIT(26)
#define BIT_27								MKBIT(27)
#define BIT_28								MKBIT(28)
#define BIT_29								MKBIT(29)
#define BIT_30								MKBIT(30)
#define BIT_31								MKBIT(31)
#define BIT_32								MKBIT(32)
#define BIT_33								MKBIT(33)
#define BIT_34								MKBIT(34)
#define BIT_35								MKBIT(35)
#define BIT_36								MKBIT(36)
#define BIT_37								MKBIT(37)
#define BIT_38								MKBIT(38)
#define BIT_39								MKBIT(39)
#define BIT_40								MKBIT(40)
#define BIT_41								MKBIT(41)
#define BIT_42								MKBIT(42)
#define BIT_43								MKBIT(43)
#define BIT_44								MKBIT(44)
#define BIT_45								MKBIT(45)
#define BIT_46								MKBIT(46)
#define BIT_47								MKBIT(47)
#define BIT_48								MKBIT(48)
#define BIT_49								MKBIT(49)
#define BIT_50								MKBIT(50)
#define BIT_51								MKBIT(51)
#define BIT_52								MKBIT(52)
#define BIT_53								MKBIT(53)
#define BIT_54								MKBIT(54)
#define BIT_55								MKBIT(55)
#define BIT_56								MKBIT(56)
#define BIT_57								MKBIT(57)
#define BIT_58								MKBIT(58)
#define BIT_59								MKBIT(59)
#define BIT_60								MKBIT(60)
#define BIT_61								MKBIT(61)
#define BIT_62								MKBIT(62)
#define BIT_63								MKBIT(63)
#define BIT_64								MKBIT(64)


#if (LS_SWAP_USE_INTRINSICS)
#	include "./intrinsics.h"
#endif


#if ((LS_GCC || LS_LLVM) && LS_INTRINSICS)
#	define LS_SWAP_16(x)					__builtin_bswap16(x)
#	define LS_SWAP_32(x)					__builtin_bswap32(x)
#	define LS_SWAP_64(x)					__builtin_bswap64(x)
#else
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


#include "./detect_endianness.h"


#if (LS_BIG_ENDIAN)
#	define LS_SWAP_ENDIAN_BIG_16(x)			(x) // already big endian
#	define LS_SWAP_ENDIAN_BIG_32(x)			(x) // already big endian
#	define LS_SWAP_ENDIAN_BIG_64(x)			(x) // already big endian
#	define LS_SWAP_ENDIAN_LITTLE_16(x)		LS_SWAP_16((x))
#	define LS_SWAP_ENDIAN_LITTLE_32(x)		LS_SWAP_32((x))
#	define LS_SWAP_ENDIAN_LITTLE_64(x)		LS_SWAP_64((x))
#else
#	define LS_SWAP_ENDIAN_BIG_16(x)			LS_SWAP_16((x))
#	define LS_SWAP_ENDIAN_BIG_32(x)			LS_SWAP_32((x))
#	define LS_SWAP_ENDIAN_BIG_64(x)			LS_SWAP_64((x))
#	define LS_SWAP_ENDIAN_LITTLE_16(x)		(x) // already little endian
#	define LS_SWAP_ENDIAN_LITTLE_32(x)		(x) // already little endian
#	define LS_SWAP_ENDIAN_LITTLE_64(x)		(x) // already little endian
#endif


#endif
