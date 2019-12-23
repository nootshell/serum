#include "./md5.h"

#include "../../core/memory.h"

#include <string.h>




unsigned int
serum_md5_init(struct serum_md5_context *const ctx) {
	SERUM_CHECK_NULLPTR(ctx);

	/* Set initial MD5 state. */
	ctx->state[0] = 0x67452301U;
	ctx->state[1] = 0xEFCDAB89U;
	ctx->state[2] = 0x98BADCFEU;
	ctx->state[3] = 0x10325476U;

	return SERUM_OK;
}


unsigned int
serum_md5_clear(struct serum_md5_context *const ctx) {
	SERUM_CHECK_NULLPTR(ctx);

	// TODO: make hash clearing erase-optional
	serum_erase(ctx, sizeof(*ctx));

	return SERUM_OK;
}




#define MD5R1( a, b, c, d, x, t, s)			(a) = (SERUM_ROTL32(((a) + (((d) ^ ((b) & ((c) ^ (d)))) + (x) + (t))), (s)) + (b))
#define MD5R2( a, b, c, d, x, t, s)			(a) = (SERUM_ROTL32(((a) + (((c) ^ ((d) & ((b) ^ (c)))) + (x) + (t))), (s)) + (b))
#define MD5R3A(a, b, c, d, x, t, s)			(a) = (SERUM_ROTL32(((a) + ((((b) ^ (c)) ^ (d)) + (x) + (t))), (s)) + (b))
#define MD5R3B(a, b, c, d, x, t, s)			(a) = (SERUM_ROTL32(((a) + (((b) ^ ((c) ^ (d))) + (x) + (t))), (s)) + (b))
#define MD5R4( a, b, c, d, x, t, s)			(a) = (SERUM_ROTL32(((a) + (((c) ^ ((b) | ~(d))) + (x) + (t))), (s)) + (b))

unsigned int
serum_md5_update(struct serum_md5_context *const SATTR_RESTRICT ctx, const void *const SATTR_RESTRICT block) {
	SERUM_CHECK_NULLPTR(ctx);
	SERUM_CHECK_NULLPTR(block);


	register uint32_t a, b, c, d;

	/* Pull state from context. */
	const uint32_t
		prev_a = a = ctx->state[0],
		prev_b = b = ctx->state[1],
		prev_c = c = ctx->state[2],
		prev_d = d = ctx->state[3];


	const uint32_t *const block32 = (const uint32_t *const)block;

#if (!SERUM_ENDIAN_LITTLE)
	uint32_t block32_conv[SERUM_MD5_BLOCK_SIZE / sizeof(uint32_t)];
	for (a = (sizeof(block32_conv) / sizeof(*block32_conv)); a--;) {
		block32_conv[a] = SERUM_LITTLE_32(block[a]);
	}
	block32 = block32_conv;
#endif


	/* Perform the first round. */
	MD5R1( a, b, c, d, block32[ 0], 0xD76AA478U,  7);
	MD5R1( d, a, b, c, block32[ 1], 0xE8C7B756U, 12);
	MD5R1( c, d, a, b, block32[ 2], 0x242070DBU, 17);
	MD5R1( b, c, d, a, block32[ 3], 0xC1BDCEEEU, 22);
	MD5R1( a, b, c, d, block32[ 4], 0xF57C0FAFU,  7);
	MD5R1( d, a, b, c, block32[ 5], 0x4787C62AU, 12);
	MD5R1( c, d, a, b, block32[ 6], 0xA8304613U, 17);
	MD5R1( b, c, d, a, block32[ 7], 0xFD469501U, 22);
	MD5R1( a, b, c, d, block32[ 8], 0x698098D8U,  7);
	MD5R1( d, a, b, c, block32[ 9], 0x8B44F7AFU, 12);
	MD5R1( c, d, a, b, block32[10], 0xFFFF5BB1U, 17);
	MD5R1( b, c, d, a, block32[11], 0x895CD7BEU, 22);
	MD5R1( a, b, c, d, block32[12], 0x6B901122U,  7);
	MD5R1( d, a, b, c, block32[13], 0xFD987193U, 12);
	MD5R1( c, d, a, b, block32[14], 0xA679438EU, 17);
	MD5R1( b, c, d, a, block32[15], 0x49B40821U, 22);

	/* Perform the second round. */
	MD5R2( a, b, c, d, block32[ 1], 0xF61E2562U,  5);
	MD5R2( d, a, b, c, block32[ 6], 0xC040B340U,  9);
	MD5R2( c, d, a, b, block32[11], 0x265E5A51U, 14);
	MD5R2( b, c, d, a, block32[ 0], 0xE9B6C7AAU, 20);
	MD5R2( a, b, c, d, block32[ 5], 0xD62F105DU,  5);
	MD5R2( d, a, b, c, block32[10], 0x02441453U,  9);
	MD5R2( c, d, a, b, block32[15], 0xD8A1E681U, 14);
	MD5R2( b, c, d, a, block32[ 4], 0xE7D3FBC8U, 20);
	MD5R2( a, b, c, d, block32[ 9], 0x21E1CDE6U,  5);
	MD5R2( d, a, b, c, block32[14], 0xC33707D6U,  9);
	MD5R2( c, d, a, b, block32[ 3], 0xF4D50D87U, 14);
	MD5R2( b, c, d, a, block32[ 8], 0x455A14EDU, 20);
	MD5R2( a, b, c, d, block32[13], 0xA9E3E905U,  5);
	MD5R2( d, a, b, c, block32[ 2], 0xFCEFA3F8U,  9);
	MD5R2( c, d, a, b, block32[ 7], 0x676F02D9U, 14);
	MD5R2( b, c, d, a, block32[12], 0x8D2A4C8AU, 20);

	/* Perform the third round. */
	MD5R3A(a, b, c, d, block32[ 5], 0xFFFA3942U,  4);
	MD5R3B(d, a, b, c, block32[ 8], 0x8771F681U, 11);
	MD5R3A(c, d, a, b, block32[11], 0x6D9D6122U, 16);
	MD5R3B(b, c, d, a, block32[14], 0xFDE5380CU, 23);
	MD5R3A(a, b, c, d, block32[ 1], 0xA4BEEA44U,  4);
	MD5R3B(d, a, b, c, block32[ 4], 0x4BDECFA9U, 11);
	MD5R3A(c, d, a, b, block32[ 7], 0xF6BB4B60U, 16);
	MD5R3B(b, c, d, a, block32[10], 0xBEBFBC70U, 23);
	MD5R3A(a, b, c, d, block32[13], 0x289B7EC6U,  4);
	MD5R3B(d, a, b, c, block32[ 0], 0xEAA127FAU, 11);
	MD5R3A(c, d, a, b, block32[ 3], 0xD4EF3085U, 16);
	MD5R3B(b, c, d, a, block32[ 6], 0x04881D05U, 23);
	MD5R3A(a, b, c, d, block32[ 9], 0xD9D4D039U,  4);
	MD5R3B(d, a, b, c, block32[12], 0xE6DB99E5U, 11);
	MD5R3A(c, d, a, b, block32[15], 0x1FA27CF8U, 16);
	MD5R3B(b, c, d, a, block32[ 2], 0xC4AC5665U, 23);

	/* Perform the fourth round. */
	MD5R4( a, b, c, d, block32[ 0], 0xF4292244U,  6);
	MD5R4( d, a, b, c, block32[ 7], 0x432AFF97U, 10);
	MD5R4( c, d, a, b, block32[14], 0xAB9423A7U, 15);
	MD5R4( b, c, d, a, block32[ 5], 0xFC93A039U, 21);
	MD5R4( a, b, c, d, block32[12], 0x655B59C3U,  6);
	MD5R4( d, a, b, c, block32[ 3], 0x8F0CCC92U, 10);
	MD5R4( c, d, a, b, block32[10], 0xFFEFF47DU, 15);
	MD5R4( b, c, d, a, block32[ 1], 0x85845DD1U, 21);
	MD5R4( a, b, c, d, block32[ 8], 0x6FA87E4FU,  6);
	MD5R4( d, a, b, c, block32[15], 0xFE2CE6E0U, 10);
	MD5R4( c, d, a, b, block32[ 6], 0xA3014314U, 15);
	MD5R4( b, c, d, a, block32[13], 0x4E0811A1U, 21);
	MD5R4( a, b, c, d, block32[ 4], 0xF7537E82U,  6);
	MD5R4( d, a, b, c, block32[11], 0xBD3AF235U, 10);
	MD5R4( c, d, a, b, block32[ 2], 0x2AD7D2BBU, 15);
	MD5R4( b, c, d, a, block32[ 9], 0xEB86D391U, 21);


	/* Update state in context. */
	ctx->state[0] = (prev_a + a);
	ctx->state[1] = (prev_b + b);
	ctx->state[2] = (prev_c + c);
	ctx->state[3] = (prev_d + d);


	return SERUM_OK;
}

#undef MD5R1
#undef MD5R2
#undef MD5R3A
#undef MD5R3B
#undef MD5R4




unsigned int
serum_md5_finish(struct serum_md5_context *const SATTR_RESTRICT ctx, unsigned char *const SATTR_RESTRICT digest, const void *const SATTR_RESTRICT block, const unsigned int remain, const unsigned long long int total_bytes) {
	SERUM_CHECK_NULLPTR(ctx);
	SERUM_CHECK_NULLPTR(digest);

	SERUM_CHECK_RANGE(remain, 0, 63);
	if (remain > 0) {
		SERUM_CHECK_NULLPTR(block);
	}


	/* Prepare buffer with remaining data and pad it. */
	unsigned char buffer[SERUM_MD5_BLOCK_SIZE];
	if (block != NULL) {
		memcpy(buffer, block, remain); // TODO: find elegant way of basically doing &0x3F
	}
	buffer[remain] = 0x80;


	/* Delta is defined as difference between buffer size and remaining (plus pad) size.*/
	const unsigned int remain_padded = (remain + 1);
	unsigned int delta = (sizeof(buffer) - remain_padded);

	/* Check if there is space for the message length (64-bit).
	** If not, zeroize buffer remains, update, and reset delta. */
	if (delta < sizeof(uint64_t)) {
		if (delta > 0) {
			memset(&buffer[remain_padded], 0, delta);
		}

		serum_md5_update(ctx, buffer);

		delta = sizeof(buffer);
	}


	/* Prepare final buffer by zeroizing from pad start to message length start and insert message length in bits. */
	memset(&buffer[sizeof(buffer) - delta], 0, (delta - sizeof(uint64_t)));
	((uint64_t*)buffer)[(SERUM_MD5_BLOCK_SIZE / sizeof(uint64_t)) - 1] = SERUM_LITTLE_64((uint64_t)(total_bytes << 3));


	/* Final update. */
	serum_md5_update(ctx, buffer);


	/* Export digest. */
#if (SERUM_ENDIAN_LITTLE)
	memcpy(digest, ctx->state, sizeof(ctx->state));
#else
	uint32_t *const d32 = (uint32_t*)digest;
	*d32[0] = SERUM_LITTLE_32(ctx->state[0]);
	*d32[1] = SERUM_LITTLE_32(ctx->state[1]);
	*d32[2] = SERUM_LITTLE_32(ctx->state[2]);
	*d32[3] = SERUM_LITTLE_32(ctx->state[3]);
#endif

	return SERUM_OK;
}
