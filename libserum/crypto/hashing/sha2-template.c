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
**    Template file, used by sha2.c
**
*/


#if ((defined(LS_SHA2_224) || defined(LS_SHA2_256)) && !defined(SHA2_32))
#	define SHA2_32
#elif ((defined(LS_SHA2_384) || defined(LS_SHA2_512)) && !defined(SHA2_64))
#	define SHA2_64
#endif

#ifdef SHA2_BITS
#undef SHA2_BITS
#endif

#ifdef SHA2_NATIVE_TYPE
#undef SHA2_NATIVE_TYPE
#endif

#ifdef SHA2_CTX
#undef SHA2_CTX
#endif

#ifdef SHA2_INIT
#undef SHA2_INIT
#endif

#ifdef SHA2_CLEAR
#undef SHA2_CLEAR
#endif

#ifdef SHA2_UPDATE
#undef SHA2_UPDATE
#endif

#ifdef SHA2_UPDATE_BLOCK
#undef SHA2_UPDATE_BLOCK
#endif

#ifdef SHA2_FINISH
#undef SHA2_FINISH
#endif

#ifdef SHA2_DIGEST_SIZE
#undef SHA2_DIGEST_SIZE
#endif

#ifdef SHA2_BLOCK_SIZE
#undef SHA2_BLOCK_SIZE
#endif

#if (defined(LS_SHA2_224))
#	define SHA2_BITS						32
#	define SHA2_NATIVE_TYPE					uint32_t
#	define SHA2_CTX							struct ls_sha2_32
#	define SHA2_INIT						ls_sha2_224_init
#	define SHA2_CLEAR						ls_sha2_224_clear
#	define SHA2_UPDATE						ls_sha2_224_update
#	define SHA2_UPDATE_BLOCK				ls_sha2_32_update_block
#	define SHA2_FINISH						ls_sha2_224_finish
#	define SHA2_DIGEST_SIZE					LS_SHA2_224_DIGEST_SIZE
#	define SHA2_BLOCK_SIZE					LS_SHA2_224_BLOCK_SIZE
#elif (defined(LS_SHA2_256))
#	define SHA2_BITS						32
#	define SHA2_NATIVE_TYPE					uint32_t
#	define SHA2_CTX							struct ls_sha2_32
#	define SHA2_INIT						ls_sha2_256_init
#	define SHA2_CLEAR						ls_sha2_256_clear
#	define SHA2_UPDATE						ls_sha2_256_update
#	define SHA2_UPDATE_BLOCK				ls_sha2_32_update_block
#	define SHA2_FINISH						ls_sha2_256_finish
#	define SHA2_DIGEST_SIZE					LS_SHA2_256_DIGEST_SIZE
#	define SHA2_BLOCK_SIZE					LS_SHA2_256_BLOCK_SIZE
#elif (defined(LS_SHA2_384))
#	define SHA2_BITS						64
#	define SHA2_NATIVE_TYPE					uint64_t
#	define SHA2_CTX							struct ls_sha2_64
#	define SHA2_INIT						ls_sha2_384_init
#	define SHA2_CLEAR						ls_sha2_384_clear
#	define SHA2_UPDATE						ls_sha2_384_update
#	define SHA2_UPDATE_BLOCK				ls_sha2_64_update_block
#	define SHA2_FINISH						ls_sha2_384_finish
#	define SHA2_DIGEST_SIZE					LS_SHA2_384_DIGEST_SIZE
#	define SHA2_BLOCK_SIZE					LS_SHA2_384_BLOCK_SIZE
#elif (defined(LS_SHA2_512))
#	define SHA2_BITS						64
#	define SHA2_NATIVE_TYPE					uint64_t
#	define SHA2_CTX							struct ls_sha2_64
#	define SHA2_INIT						ls_sha2_512_init
#	define SHA2_CLEAR						ls_sha2_512_clear
#	define SHA2_UPDATE						ls_sha2_512_update
#	define SHA2_UPDATE_BLOCK				ls_sha2_64_update_block
#	define SHA2_FINISH						ls_sha2_512_finish
#	define SHA2_DIGEST_SIZE					LS_SHA2_512_DIGEST_SIZE
#	define SHA2_BLOCK_SIZE					LS_SHA2_512_BLOCK_SIZE
#endif

#ifdef SHA2_BITS
#define SHA2_BYTES							(SHA2_BITS / 8)
#endif


#if (defined(SHA2_32) && !defined(SHA2_32_C))
#	define SHA2_32_C

static const uint32_t constants32[] = {
	0x428A2F98U, 0x71374491U, 0xB5C0FBCFU, 0xE9B5DBA5U,
	0x3956C25BU, 0x59F111F1U, 0x923F82A4U, 0xAB1C5ED5U,
	0xD807AA98U, 0x12835B01U, 0x243185BEU, 0x550C7DC3U,
	0x72BE5D74U, 0x80DEB1FEU, 0x9BDC06A7U, 0xC19BF174U,
	0xE49B69C1U, 0xEFBE4786U, 0x0FC19DC6U, 0x240CA1CCU,
	0x2DE92C6FU, 0x4A7484AAU, 0x5CB0A9DCU, 0x76F988DAU,
	0x983E5152U, 0xA831C66DU, 0xB00327C8U, 0xBF597FC7U,
	0xC6E00BF3U, 0xD5A79147U, 0x06CA6351U, 0x14292967U,
	0x27B70A85U, 0x2E1B2138U, 0x4D2C6DFCU, 0x53380D13U,
	0x650A7354U, 0x766A0ABBU, 0x81C2C92EU, 0x92722C85U,
	0xA2BFE8A1U, 0xA81A664BU, 0xC24B8B70U, 0xC76C51A3U,
	0xD192E819U, 0xD6990624U, 0xF40E3585U, 0x106AA070U,
	0x19A4C116U, 0x1E376C08U, 0x2748774CU, 0x34B0BCB5U,
	0x391C0CB3U, 0x4ED8AA4AU, 0x5B9CCA4FU, 0x682E6FF3U,
	0x748F82EEU, 0x78A5636FU, 0x84C87814U, 0x8CC70208U,
	0x90BEFFFAU, 0xA4506CEBU, 0xBEF9A3F7U, 0xC67178F2U
};

#	define SHA2_CONSTANTS					constants32
#	define SHA2_WR							64
#	define SHA2_ROTR						LS_ROTR32
#	define SHA2_ROTR_1						7
#	define SHA2_ROTR_2						18
#	define SHA2_ROTR_3						17
#	define SHA2_ROTR_4						19
#	define SHA2_ROTR_5						6
#	define SHA2_ROTR_6						11
#	define SHA2_ROTR_7						25
#	define SHA2_ROTR_8						2
#	define SHA2_ROTR_9						13
#	define SHA2_ROTR_10						22
#	define SHA2_SHR_1						3
#	define SHA2_SHR_2						10
#	include "./sha2-template-update.c"
#	undef SHA2_CONSTANTS
#	undef SHA2_WR
#	undef SHA2_ROTR
#	undef SHA2_ROTR_1
#	undef SHA2_ROTR_2
#	undef SHA2_ROTR_3
#	undef SHA2_ROTR_4
#	undef SHA2_ROTR_5
#	undef SHA2_ROTR_6
#	undef SHA2_ROTR_7
#	undef SHA2_ROTR_8
#	undef SHA2_ROTR_9
#	undef SHA2_ROTR_10
#	undef SHA2_SHR_1
#	undef SHA2_SHR_2
#elif (defined(SHA2_64) && !defined(SHA2_64_C))
#	define SHA2_64_C

static const uint64_t constants64[] = {
	0x428A2F98D728AE22U, 0x7137449123EF65CDU, 0xB5C0FBCFEC4D3B2FU, 0xE9B5DBA58189DBBCU,
	0x3956C25BF348B538U, 0x59F111F1B605D019U, 0x923F82A4AF194F9BU, 0xAB1C5ED5DA6D8118U,
	0xD807AA98A3030242U, 0x12835B0145706FBEU, 0x243185BE4EE4B28CU, 0x550C7DC3D5FFB4E2U,
	0x72BE5D74F27B896FU, 0x80DEB1FE3B1696B1U, 0x9BDC06A725C71235U, 0xC19BF174CF692694U,
	0xE49B69C19EF14AD2U, 0xEFBE4786384F25E3U, 0x0FC19DC68B8CD5B5U, 0x240CA1CC77AC9C65U,
	0x2DE92C6F592B0275U, 0x4A7484AA6EA6E483U, 0x5CB0A9DCBD41FBD4U, 0x76F988DA831153B5U,
	0x983E5152EE66DFABU, 0xA831C66D2DB43210U, 0xB00327C898FB213FU, 0xBF597FC7BEEF0EE4U,
	0xC6E00BF33DA88FC2U, 0xD5A79147930AA725U, 0x06CA6351E003826FU, 0x142929670A0E6E70U,
	0x27B70A8546D22FFCU, 0x2E1B21385C26C926U, 0x4D2C6DFC5AC42AEDU, 0x53380D139D95B3DFU,
	0x650A73548BAF63DEU, 0x766A0ABB3C77B2A8U, 0x81C2C92E47EDAEE6U, 0x92722C851482353BU,
	0xA2BFE8A14CF10364U, 0xA81A664BBC423001U, 0xC24B8B70D0F89791U, 0xC76C51A30654BE30U,
	0xD192E819D6EF5218U, 0xD69906245565A910U, 0xF40E35855771202AU, 0x106AA07032BBD1B8U,
	0x19A4C116B8D2D0C8U, 0x1E376C085141AB53U, 0x2748774CDF8EEB99U, 0x34B0BCB5E19B48A8U,
	0x391C0CB3C5C95A63U, 0x4ED8AA4AE3418ACBU, 0x5B9CCA4F7763E373U, 0x682E6FF3D6B2B8A3U,
	0x748F82EE5DEFB2FCU, 0x78A5636F43172F60U, 0x84C87814A1F0AB72U, 0x8CC702081A6439ECU,
	0x90BEFFFA23631E28U, 0xA4506CEBDE82BDE9U, 0xBEF9A3F7B2C67915U, 0xC67178F2E372532BU,
	0xCA273ECEEA26619CU, 0xD186B8C721C0C207U, 0xEADA7DD6CDE0EB1EU, 0xF57D4F7FEE6ED178U,
	0x06F067AA72176FBAU, 0x0A637DC5A2C898A6U, 0x113F9804BEF90DAEU, 0x1B710B35131C471BU,
	0x28DB77F523047D84U, 0x32CAAB7B40C72493U, 0x3C9EBE0A15C9BEBCU, 0x431D67C49C100D4CU,
	0x4CC5D4BECB3E42B6U, 0x597F299CFC657E2AU, 0x5FCB6FAB3AD6FAECU, 0x6C44198C4A475817U
};

#	define SHA2_CONSTANTS					constants64
#	define SHA2_WR							80
#	define SHA2_ROTR						LS_ROTR64
#	define SHA2_ROTR_1						1
#	define SHA2_ROTR_2						8
#	define SHA2_ROTR_3						19
#	define SHA2_ROTR_4						61
#	define SHA2_ROTR_5						14
#	define SHA2_ROTR_6						18
#	define SHA2_ROTR_7						41
#	define SHA2_ROTR_8						28
#	define SHA2_ROTR_9						34
#	define SHA2_ROTR_10						39
#	define SHA2_SHR_1						7
#	define SHA2_SHR_2						6
#	include "./sha2-template-update.c"
#	undef SHA2_CONSTANTS
#	undef SHA2_WR
#	undef SHA2_ROTR
#	undef SHA2_ROTR_1
#	undef SHA2_ROTR_2
#	undef SHA2_ROTR_3
#	undef SHA2_ROTR_4
#	undef SHA2_ROTR_5
#	undef SHA2_ROTR_6
#	undef SHA2_ROTR_7
#	undef SHA2_ROTR_8
#	undef SHA2_ROTR_9
#	undef SHA2_ROTR_10
#	undef SHA2_SHR_1
#	undef SHA2_SHR_2
#endif


#ifdef SHA2_CTX
ls_result_t
SHA2_INIT(SHA2_CTX *const ctx) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	SHA2_CLEAR(ctx);

	ctx->pad = 0x80;

#if (defined(LS_SHA2_224))
	ctx->h[0] = 0xC1059ED8U;
	ctx->h[1] = 0x367CD507U;
	ctx->h[2] = 0x3070DD17U;
	ctx->h[3] = 0xF70E5939U;
	ctx->h[4] = 0xFFC00B31U;
	ctx->h[5] = 0x68581511U;
	ctx->h[6] = 0x64F98FA7U;
	ctx->h[7] = 0xBEFA4FA4U;
#elif (defined(LS_SHA2_256))
	ctx->h[0] = 0x6A09E667U;
	ctx->h[1] = 0xBB67AE85U;
	ctx->h[2] = 0x3C6EF372U;
	ctx->h[3] = 0xA54FF53AU;
	ctx->h[4] = 0x510E527FU;
	ctx->h[5] = 0x9B05688CU;
	ctx->h[6] = 0x1F83D9ABU;
	ctx->h[7] = 0x5BE0CD19U;
#elif (defined(LS_SHA2_384))
	ctx->h[0] = 0xCBBB9D5DC1059ED8U;
	ctx->h[1] = 0x629A292A367CD507U;
	ctx->h[2] = 0x9159015A3070DD17U;
	ctx->h[3] = 0x152FECD8F70E5939U;
	ctx->h[4] = 0x67332667FFC00B31U;
	ctx->h[5] = 0x8EB44A8768581511U;
	ctx->h[6] = 0xDB0C2E0D64F98FA7U;
	ctx->h[7] = 0X47B5481DBEFA4FA4U;
#elif (defined(LS_SHA2_512))
	ctx->h[0] = 0x6A09E667F3BCC908U;
	ctx->h[1] = 0xBB67AE8584CAA73BU;
	ctx->h[2] = 0x3C6EF372FE94F82BU;
	ctx->h[3] = 0xA54FF53A5F1D36F1U;
	ctx->h[4] = 0x510E527FADE682D1U;
	ctx->h[5] = 0x9B05688C2B3E6C1FU;
	ctx->h[6] = 0x1F83D9ABFB41BD6BU;
	ctx->h[7] = 0x5BE0CD19137E2179U;
#else
#	error SHA-2-?
#endif

	return LS_RESULT_SUCCESS;
}


ls_result_t
SHA2_CLEAR(SHA2_CTX *const ctx) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	memset(ctx, 0, sizeof(*ctx));

	return LS_RESULT_SUCCESS;
}


ls_result_t
SHA2_UPDATE(SHA2_CTX *const restrict ctx, const void *const restrict in, size_t size) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(in, 2);
	LS_RESULT_CHECK_SIZE(size, 1);

	ctx->size += size;

	if (size == (sizeof(SHA2_NATIVE_TYPE) * 16)) {
		return SHA2_UPDATE_BLOCK(ctx, in);
	} else {
		if ((ctx->__psize + size) <= sizeof(ctx->__pcache)) {
			memcpy(ctx->__pcache + ctx->__psize, in, size);
			ctx->__psize += size;
			return LS_RESULT_SUCCESS_CODE(LS_RESULT_CODE_EARLY_EXIT);
		}

		SHA2_NATIVE_TYPE buffer[LS_SHA2_BLOCK_NUM];

		const uint8_t *iptr = in;

		// check if partial data
		if (ctx->__psize) {
			memcpy(buffer, ctx->__pcache, ctx->__psize);

			size_t from_input = (sizeof(buffer) - ctx->__psize);
			memcpy(((uint8_t*)buffer) + ctx->__psize, iptr, from_input);
			iptr += from_input;
			size += ctx->__psize;
		} else {
			memcpy(buffer, iptr, sizeof(buffer));
			iptr += sizeof(buffer);
		}

		// while have full block, update block
		do {
			SHA2_UPDATE_BLOCK(ctx, buffer);
			if ((size -= sizeof(buffer)) >= sizeof(buffer)) {
				memcpy(buffer, iptr, sizeof(buffer));
				iptr += sizeof(buffer);
			} else {
				break;
			}
		} while (true);

		// if partial block, cache
		if (ctx->__psize = (uint32_t)size) {
			memcpy(ctx->__pcache, iptr, size);
			return LS_RESULT_SUCCESS_CODE(LS_RESULT_CODE_SIZE);
		}

		return LS_RESULT_SUCCESS;
	}
}

#include <stdio.h>
ls_result_t
SHA2_FINISH(SHA2_CTX *const ctx, uint8_t digest[SHA2_DIGEST_SIZE]) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(digest, 2);


	uint8_t buffer[SHA2_BLOCK_SIZE];
	uintptr_t offset = 0;

	if (ctx->__psize) {
		memcpy(buffer, ctx->__pcache, ctx->__psize);
		offset = ctx->__psize;
	}

	size_t diff = (sizeof(buffer) - offset);
	memset(buffer + offset, 0, diff);
	buffer[offset] = ctx->pad;

	if (diff < 9) {
		SHA2_UPDATE_BLOCK(ctx, (void*)buffer);
		memset(buffer, 0, sizeof(buffer));
	}

	buffer[sizeof(buffer) - 8] = ((ctx->size >> 56) & 0xFF);
	buffer[sizeof(buffer) - 7] = ((ctx->size >> 48) & 0xFF);
	buffer[sizeof(buffer) - 6] = ((ctx->size >> 40) & 0xFF);
	buffer[sizeof(buffer) - 5] = ((ctx->size >> 32) & 0xFF);
	buffer[sizeof(buffer) - 4] = ((ctx->size >> 24) & 0xFF);
	buffer[sizeof(buffer) - 3] = ((ctx->size >> 16) & 0xFF);
	buffer[sizeof(buffer) - 2] = ((ctx->size >>  8) & 0xFF);
	buffer[sizeof(buffer) - 1] = ((ctx->size      ) & 0xFF);


	SHA2_UPDATE_BLOCK(ctx, (void*)buffer);


	uint_fast8_t i;
	for (i = SHA2_BYTES; i--;) {                                                           	// 224 256 384 512
		digest[i                   ] = ((ctx->h[0] >> ((SHA2_BITS - 8) - (8 * i))) & 0xFF); //  y   y   y   y
		digest[i + (SHA2_BYTES * 1)] = ((ctx->h[1] >> ((SHA2_BITS - 8) - (8 * i))) & 0xFF); //  y   y   y   y
		digest[i + (SHA2_BYTES * 2)] = ((ctx->h[2] >> ((SHA2_BITS - 8) - (8 * i))) & 0xFF); //  y   y   y   y
		digest[i + (SHA2_BYTES * 3)] = ((ctx->h[3] >> ((SHA2_BITS - 8) - (8 * i))) & 0xFF); //  y   y   y   y
		digest[i + (SHA2_BYTES * 4)] = ((ctx->h[4] >> ((SHA2_BITS - 8) - (8 * i))) & 0xFF); //  y   y   y   y
		digest[i + (SHA2_BYTES * 5)] = ((ctx->h[5] >> ((SHA2_BITS - 8) - (8 * i))) & 0xFF); //  y   y   y   y
#if (defined(LS_SHA2_512) || defined(LS_SHA2_256) || defined(LS_SHA2_224))
		digest[i + (SHA2_BYTES * 6)] = ((ctx->h[6] >> ((SHA2_BITS - 8) - (8 * i))) & 0xFF); //  y   y   n   y
#endif
#if (defined(LS_SHA2_512) || defined(LS_SHA2_256))
		digest[i + (SHA2_BYTES * 7)] = ((ctx->h[7] >> ((SHA2_BITS - 8) - (8 * i))) & 0xFF); //  n   y   n   y
#endif
	}


	return LS_RESULT_SUCCESS;
}
#endif
