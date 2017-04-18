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

#define FILE_PATH							"crypto/hashing/md5.c"

#include "./md5.h"
#include <string.h>


ID("MD5 implementation");


#define MD5R1( a, b, c, d, x, t, s)			(a) = (LS_ROTL32(((a) + (((d) ^ ((b) & ((c) ^ (d)))) + (x) + (t))), (s)) + (b))
#define MD5R2( a, b, c, d, x, t, s)			(a) = (LS_ROTL32(((a) + (((c) ^ ((d) & ((b) ^ (c)))) + (x) + (t))), (s)) + (b))
#define MD5R3A(a, b, c, d, x, t, s)			(a) = (LS_ROTL32(((a) + ((((b) ^ (c)) ^ (d)) + (x) + (t))), (s)) + (b))
#define MD5R3B(a, b, c, d, x, t, s)			(a) = (LS_ROTL32(((a) + (((b) ^ ((c) ^ (d))) + (x) + (t))), (s)) + (b))
#define MD5R4( a, b, c, d, x, t, s)			(a) = (LS_ROTL32(((a) + (((c) ^ ((b) | ~(d))) + (x) + (t))), (s)) + (b))


void
static ls_md5_update_block(ls_md5_t *const LS_RESTRICT ctx, const uint32_t *const LS_RESTRICT block) {
	register uint32_t a, b, c, d;
	uint32_t a_old, b_old, c_old, d_old;
	const uint32_t *const ptr = block;

#if (!LS_LITTLE_ENDIAN)
	uint32_t le_block[LS_MD5_BLOCK_SIZE / sizeof(uint32_t)];
	for (a = (sizeof(le_block) / sizeof(*le_block)); a--;) {
		le_block[a] = LS_SWAP_ENDIAN_LITTLE_32(block[a]);
	}
	ptr = le_block;
#endif

	a_old = (a = ctx->h[0]);
	b_old = (b = ctx->h[1]);
	c_old = (c = ctx->h[2]);
	d_old = (d = ctx->h[3]);

	MD5R1 (a, b, c, d, ptr[ 0], 0xD76AA478U,  7);
	MD5R1 (d, a, b, c, ptr[ 1], 0xE8C7B756U, 12);
	MD5R1 (c, d, a, b, ptr[ 2], 0x242070DBU, 17);
	MD5R1 (b, c, d, a, ptr[ 3], 0xC1BDCEEEU, 22);
	MD5R1 (a, b, c, d, ptr[ 4], 0xF57C0FAFU,  7);
	MD5R1 (d, a, b, c, ptr[ 5], 0x4787C62AU, 12);
	MD5R1 (c, d, a, b, ptr[ 6], 0xA8304613U, 17);
	MD5R1 (b, c, d, a, ptr[ 7], 0xFD469501U, 22);
	MD5R1 (a, b, c, d, ptr[ 8], 0x698098D8U,  7);
	MD5R1 (d, a, b, c, ptr[ 9], 0x8B44F7AFU, 12);
	MD5R1 (c, d, a, b, ptr[10], 0xFFFF5BB1U, 17);
	MD5R1 (b, c, d, a, ptr[11], 0x895CD7BEU, 22);
	MD5R1 (a, b, c, d, ptr[12], 0x6B901122U,  7);
	MD5R1 (d, a, b, c, ptr[13], 0xFD987193U, 12);
	MD5R1 (c, d, a, b, ptr[14], 0xA679438EU, 17);
	MD5R1 (b, c, d, a, ptr[15], 0x49B40821U, 22);

	MD5R2 (a, b, c, d, ptr[ 1], 0xF61E2562U,  5);
	MD5R2 (d, a, b, c, ptr[ 6], 0xC040B340U,  9);
	MD5R2 (c, d, a, b, ptr[11], 0x265E5A51U, 14);
	MD5R2 (b, c, d, a, ptr[ 0], 0xE9B6C7AAU, 20);
	MD5R2 (a, b, c, d, ptr[ 5], 0xD62F105DU,  5);
	MD5R2 (d, a, b, c, ptr[10], 0x02441453U,  9);
	MD5R2 (c, d, a, b, ptr[15], 0xD8A1E681U, 14);
	MD5R2 (b, c, d, a, ptr[ 4], 0xE7D3FBC8U, 20);
	MD5R2 (a, b, c, d, ptr[ 9], 0x21E1CDE6U,  5);
	MD5R2 (d, a, b, c, ptr[14], 0xC33707D6U,  9);
	MD5R2 (c, d, a, b, ptr[ 3], 0xF4D50D87U, 14);
	MD5R2 (b, c, d, a, ptr[ 8], 0x455A14EDU, 20);
	MD5R2 (a, b, c, d, ptr[13], 0xA9E3E905U,  5);
	MD5R2 (d, a, b, c, ptr[ 2], 0xFCEFA3F8U,  9);
	MD5R2 (c, d, a, b, ptr[ 7], 0x676F02D9U, 14);
	MD5R2 (b, c, d, a, ptr[12], 0x8D2A4C8AU, 20);

	MD5R3A(a, b, c, d, ptr[ 5], 0xFFFA3942U,  4);
	MD5R3B(d, a, b, c, ptr[ 8], 0x8771F681U, 11);
	MD5R3A(c, d, a, b, ptr[11], 0x6D9D6122U, 16);
	MD5R3B(b, c, d, a, ptr[14], 0xFDE5380CU, 23);
	MD5R3A(a, b, c, d, ptr[ 1], 0xA4BEEA44U,  4);
	MD5R3B(d, a, b, c, ptr[ 4], 0x4BDECFA9U, 11);
	MD5R3A(c, d, a, b, ptr[ 7], 0xF6BB4B60U, 16);
	MD5R3B(b, c, d, a, ptr[10], 0xBEBFBC70U, 23);
	MD5R3A(a, b, c, d, ptr[13], 0x289B7EC6U,  4);
	MD5R3B(d, a, b, c, ptr[ 0], 0xEAA127FAU, 11);
	MD5R3A(c, d, a, b, ptr[ 3], 0xD4EF3085U, 16);
	MD5R3B(b, c, d, a, ptr[ 6], 0x04881D05U, 23);
	MD5R3A(a, b, c, d, ptr[ 9], 0xD9D4D039U,  4);
	MD5R3B(d, a, b, c, ptr[12], 0xE6DB99E5U, 11);
	MD5R3A(c, d, a, b, ptr[15], 0x1FA27CF8U, 16);
	MD5R3B(b, c, d, a, ptr[ 2], 0xC4AC5665U, 23);

	MD5R4 (a, b, c, d, ptr[ 0], 0xF4292244U,  6);
	MD5R4 (d, a, b, c, ptr[ 7], 0x432AFF97U, 10);
	MD5R4 (c, d, a, b, ptr[14], 0xAB9423A7U, 15);
	MD5R4 (b, c, d, a, ptr[ 5], 0xFC93A039U, 21);
	MD5R4 (a, b, c, d, ptr[12], 0x655B59C3U,  6);
	MD5R4 (d, a, b, c, ptr[ 3], 0x8F0CCC92U, 10);
	MD5R4 (c, d, a, b, ptr[10], 0xFFEFF47DU, 15);
	MD5R4 (b, c, d, a, ptr[ 1], 0x85845DD1U, 21);
	MD5R4 (a, b, c, d, ptr[ 8], 0x6FA87E4FU,  6);
	MD5R4 (d, a, b, c, ptr[15], 0xFE2CE6E0U, 10);
	MD5R4 (c, d, a, b, ptr[ 6], 0xA3014314U, 15);
	MD5R4 (b, c, d, a, ptr[13], 0x4E0811A1U, 21);
	MD5R4 (a, b, c, d, ptr[ 4], 0xF7537E82U,  6);
	MD5R4 (d, a, b, c, ptr[11], 0xBD3AF235U, 10);
	MD5R4 (c, d, a, b, ptr[ 2], 0x2AD7D2BBU, 15);
	MD5R4 (b, c, d, a, ptr[ 9], 0xEB86D391U, 21);

	ctx->h[0] = (a_old + a);
	ctx->h[1] = (b_old + b);
	ctx->h[2] = (c_old + c);
	ctx->h[3] = (d_old + d);
}


ls_result_t
ls_md5_init(ls_md5_t *const ctx) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	ctx->h[0] = 0x67452301U;
	ctx->h[1] = 0xEFCDAB89U;
	ctx->h[2] = 0x98BADCFEU;
	ctx->h[3] = 0x10325476U;
	ctx->hi = 0;
	ctx->lo = 0;
	ctx->__psize = 0;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_md5_clear(ls_md5_t *const ctx) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	memset(ctx, 0, sizeof(*ctx));

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_md5_update(ls_md5_t *const LS_RESTRICT ctx, const void *const LS_RESTRICT data, size_t size) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(data, 2);
#if (LS_MD5_DENY_SIZE_ZERO)
	LS_RESULT_CHECK_SIZE(size, 1);
#else
	if (!size) {
		return LS_RESULT_SUCCESS_CODE(LS_RESULT_CODE_SIZE);
	}
#endif


	uint32_t lo_old = ctx->lo;
	if ((ctx->lo = ((lo_old + size) & 0x1FFFFFFF)) < lo_old) {
		++(ctx->hi);
	}
	ctx->hi += (uint32_t)(size >> 29);


	// Check if we have enough data to make a full block, if not append the input to the cache and return.
	if ((ctx->__psize + size) < sizeof(ctx->__pcache)) {
		memcpy(ctx->__pcache + ctx->__psize, data, size);
		ctx->__psize += size;
		return LS_RESULT_SUCCESS_CODE(LS_RESULT_CODE_EARLY_EXIT);
	}


	const uint8_t *iptr = data;
	uint8_t buffer[sizeof(ctx->__pcache)];


	// Check if we have partial data cached. If we do, slam it in front of the buffer.
	if (ctx->__psize) {
		memcpy(buffer, ctx->__pcache, ctx->__psize);

		size_t from_input = (sizeof(buffer) - ctx->__psize);
		memcpy(buffer + ctx->__psize, iptr, from_input);
		iptr += from_input;
		size += ctx->__psize;
	} else {
		memcpy(buffer, iptr, sizeof(buffer));
		iptr += sizeof(buffer);
	}


	// While we have at least a full block, update that block and move on to the next.
	do {
		ls_md5_update_block(ctx, (uint32_t*)buffer);
		if ((size -= sizeof(buffer)) >= sizeof(buffer)) {
			memcpy(buffer, iptr, sizeof(buffer));
			iptr += sizeof(buffer);
		} else {
			break;
		}
	} while (true);


	// If we have a partial block remaining, cache it.
	if (ctx->__psize = (uint32_t)size) {
		memcpy(ctx->__pcache, iptr, size);
		return LS_RESULT_SUCCESS_CODE(LS_RESULT_CODE_SIZE);
	}


	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_md5_finish(ls_md5_t *const ctx, ls_md5_digest_t digest) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(digest, 2);

	ctx->__pcache[ctx->__psize++] = 0x80;

	size_t diff = (sizeof(ctx->__pcache) - ctx->__psize);
	if (diff < 8) {
		if (ctx->__psize != sizeof(ctx->__pcache)) {
			memset(ctx->__pcache + ctx->__psize, 0, diff);
		}
		ls_md5_update_block(ctx, (uint32_t*)ctx->__pcache);
		ctx->__psize = 0;
		diff = sizeof(ctx->__pcache);
	}
	memset(ctx->__pcache + ctx->__psize, 0, diff - 8);

	ctx->lo <<= 3;
	*((uint32_t*)(ctx->__pcache + 56)) = LS_SWAP_ENDIAN_LITTLE_32(ctx->lo);
	*((uint32_t*)(ctx->__pcache + 60)) = LS_SWAP_ENDIAN_LITTLE_32(ctx->hi);

	ls_md5_update_block(ctx, (uint32_t*)ctx->__pcache);

#if (LS_LITTLE_ENDIAN)
	memcpy(digest, ctx->h, sizeof(ctx->h));
#else
	uint32_t *const d32 = (uint32_t*)digest;
	*d32[0] = LS_SWAP_ENDIAN_LITTLE_32(ctx->h[0]);
	*d32[1] = LS_SWAP_ENDIAN_LITTLE_32(ctx->h[1]);
	*d32[2] = LS_SWAP_ENDIAN_LITTLE_32(ctx->h[2]);
	*d32[3] = LS_SWAP_ENDIAN_LITTLE_32(ctx->h[3]);
#endif

	return LS_RESULT_SUCCESS;
}
