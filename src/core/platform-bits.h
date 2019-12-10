#ifndef __SERUM_CORE_PLATFORM_BITS_H
#define __SERUM_CORE_PLATFORM_BITS_H

#ifndef __SERUM_CORE_PLATFORM_H
#	error Expecting to be included from serum/core/platform.h, which was not detected.
#endif




#define SPF_BIT_PRESENT(__set, __sub)		(((__set) & (__sub)) == (__sub))




/* Create bitwise rotation routines. Compilers should optimize this properly. (Yes, I'm completely aware I should not rely on the compiler, this -for now- is an exception.) */

#define __MKROT(name, s1, s2, bits)													\
uint ## bits ## _t																		\
static SATTR_INLINE name(const uint ## bits ## _t value, const unsigned int shift) {	\
	return (((value) s1 (shift)) | ((value) s2 (-(shift) & ((bits) - 1))));				\
}

#define __MKROTL(bits)	__MKROT(SERUM_ROTL ## bits, <<, >>, bits)
#define __MKROTR(bits)	__MKROT(SERUM_ROTR ## bits, >>, <<, bits)

__MKROTL(8)
__MKROTL(16)
__MKROTL(32)
__MKROTL(64)

__MKROTR(8)
__MKROTR(16)
__MKROTR(32)
__MKROTR(64)

#undef __MKROT
#undef __MKROTL
#undef __MKROTR




/* Define swapping macros. */
#if ((SERUM_GCC || SERUM_LLVM) && (SPF_BUILTIN_AVAIL(__builtin_bswap16) && SPF_BUILTIN_AVAIL(__builtin_bswap32) && SPF_BUILTIN_AVAIL(__builtin_bswap64)))
#	define SERUM_SWAP_16(x)						__builtin_bswap16(x)
#	define SERUM_SWAP_32(x)						__builtin_bswap32(x)
#	define SERUM_SWAP_64(x)						__builtin_bswap64(x)
#elif (SERUM_MSC && SERUM_INTRINSICS)
#	define SERUM_SWAP_16(x)						_byteswap_ushort(x)
#	define SERUM_SWAP_32(x)						_byteswap_ulong(x)
#	define SERUM_SWAP_64(x)						_byteswap_uint64(x)
#else
#	define SERUM_SWAP_16(x) (					\
		(((x) & 0xFF00            ) >>  8) |	\
		(((x) & 0x00FF            ) <<  8)		\
	)
#	define SERUM_SWAP_32(x) (					\
		(((x) & 0xFF000000        ) >> 24) |	\
		(((x) & 0x00FF0000        ) >>  8) |	\
		(((x) & 0x0000FF00        ) <<  8) |	\
		(((x) & 0x000000FF        ) << 24)		\
	)
#	define SERUM_SWAP_64(x) (					\
		(((x) & 0xFF00000000000000) >> 56) |	\
		(((x) & 0x00FF000000000000) >> 40) |	\
		(((x) & 0x0000FF0000000000) >> 24) |	\
		(((x) & 0x000000FF00000000) >>  8) |	\
		(((x) & 0x00000000FF000000) <<  8) |	\
		(((x) & 0x0000000000FF0000) << 24) |	\
		(((x) & 0x000000000000FF00) << 40) |	\
		(((x) & 0x00000000000000FF) << 56)		\
	)
#endif




#if (SERUM_ENDIAN_BIG)
#	define SERUM_BIG_16(x)					(x) // already big endian
#	define SERUM_BIG_32(x)					(x) // already big endian
#	define SERUM_BIG_64(x)					(x) // already big endian
#	define SERUM_LITTLE_16(x)				SERUM_SWAP_16((x))
#	define SERUM_LITTLE_32(x)				SERUM_SWAP_32((x))
#	define SERUM_LITTLE_64(x)				SERUM_SWAP_64((x))
#else
#	define SERUM_BIG_16(x)					SERUM_SWAP_16((x))
#	define SERUM_BIG_32(x)					SERUM_SWAP_32((x))
#	define SERUM_BIG_64(x)					SERUM_SWAP_64((x))
#	define SERUM_LITTLE_16(x)				(x) // already little endian
#	define SERUM_LITTLE_32(x)				(x) // already little endian
#	define SERUM_LITTLE_64(x)				(x) // already little endian
#endif


#endif
