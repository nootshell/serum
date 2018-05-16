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


#include "../../../core/setup/features.h"

#if (LSCST_ENABLED)




#include "./ripemd160.h"

#include "../../../core/memory.h"
#include "../assert.h"
#include "../base.h"

#include <string.h>




FILEID("CST: RIPEMD-160");




ls_result_t
lscst_hashing_ripemd160(const lsreg_meta_t *const meta) {
	struct vector {
		ls_ripemd160_digest_t digest;
		char data[LS_RIPEMD160_BLOCK_SIZE];
		uint32_t __pad;
		size_t iterations;
		char source[32];
	} vectors[] = {
		{
			.digest = { 0x9C, 0x11, 0x85, 0xA5, 0xC5, 0xE9, 0xFC, 0x54, 0x61, 0x28, 0x08, 0x97, 0x7E, 0xE8, 0xF5, 0x48, 0xB2, 0x25, 0x8D, 0x31 },
			.data = "",
			.iterations = 1,
			.source = "The RIPEMD-160 page"
		},
		{
			.digest = { 0x0B, 0xDC, 0x9D, 0x2D, 0x25, 0x6B, 0x3E, 0xE9, 0xDA, 0xAE, 0x34, 0x7B, 0xE6, 0xF4, 0xDC, 0x83, 0x5A, 0x46, 0x7F, 0xFE },
			.data = "a",
			.iterations = 1,
			.source = "The RIPEMD-160 page"
		},
		{
			.digest = { 0x8E, 0xB2, 0x08, 0xF7, 0xE0, 0x5D, 0x98, 0x7A, 0x9B, 0x04, 0x4A, 0x8E, 0x98, 0xC6, 0xB0, 0x87, 0xF1, 0x5A, 0x0B, 0xFC },
			.data = "abc",
			.iterations = 1,
			.source = "The RIPEMD-160 page"
		},
		{
			.digest = { 0x5D, 0x06, 0x89, 0xEF, 0x49, 0xD2, 0xFA, 0xE5, 0x72, 0xB8, 0x81, 0xB1, 0x23, 0xA8, 0x5F, 0xFA, 0x21, 0x59, 0x5F, 0x36 },
			.data = "message digest",
			.iterations = 1,
			.source = "The RIPEMD-160 page"
		},
		{
			.digest = { 0xF7, 0x1C, 0x27, 0x10, 0x9C, 0x69, 0x2C, 0x1B, 0x56, 0xBB, 0xDC, 0xEB, 0x5B, 0x9D, 0x28, 0x65, 0xB3, 0x70, 0x8D, 0xBC },
			.data = "abcdefghijklmnopqrstuvwxyz",
			.iterations = 1,
			.source = "The RIPEMD-160 page"
		},
		{
			.digest = { 0x12, 0xA0, 0x53, 0x38, 0x4A, 0x9C, 0x0C, 0x88, 0xE4, 0x05, 0xA0, 0x6C, 0x27, 0xDC, 0xF4, 0x9A, 0xDA, 0x62, 0xEB, 0x2B },
			.data = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
			.iterations = 1,
			.source = "The RIPEMD-160 page"
		},
		{
			.digest = { 0xB0, 0xE2, 0x0B, 0x6E, 0x31, 0x16, 0x64, 0x02, 0x86, 0xED, 0x3A, 0x87, 0xA5, 0x71, 0x30, 0x79, 0xB2, 0x1F, 0x51, 0x89 },
			.data = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
			.iterations = 1,
			.source = "The RIPEMD-160 page"
		},
		{
			.digest = { 0x9B, 0x75, 0x2E, 0x45, 0x57, 0x3D, 0x4B, 0x39, 0xF4, 0xDB, 0xD3, 0x32, 0x3C, 0xAB, 0x82, 0xBF, 0x63, 0x32, 0x6B, 0xFB },
			.data = "1234567890",
			.iterations = 8,
			.source = "The RIPEMD-160 page"
		},
		{
			.digest = { 0x52, 0x78, 0x32, 0x43, 0xC1, 0x69, 0x7B, 0xDB, 0xE1, 0x6D, 0x37, 0xF9, 0x7F, 0x68, 0xF0, 0x83, 0x25, 0xDC, 0x15, 0x28 },
			.data = "a",
			.iterations = 1000000,
			.source = "The RIPEMD-160 page"
		},
		{
			.digest = { 0x37, 0xF3, 0x32, 0xF6, 0x8D, 0xB7, 0x7B, 0xD9, 0xD7, 0xED, 0xD4, 0x96, 0x95, 0x71, 0xAD, 0x67, 0x1C, 0xF9, 0xDD, 0x3B },
			.data = "The quick brown fox jumps over the lazy dog",
			.iterations = 1,
			.source = "Wikipedia"
		},
		{
			.digest = { 0x13, 0x20, 0x72, 0xDF, 0x69, 0x09, 0x33, 0x83, 0x5E, 0xB8, 0xB6, 0xAD, 0x0B, 0x77, 0xE7, 0xB6, 0xF1, 0x4A, 0xCA, 0xD7 },
			.data = "The quick brown fox jumps over the lazy cog",
			.iterations = 1,
			.source = "Wikipedia"
		}
	};

	const size_t nvectors = (sizeof(vectors) / sizeof(*vectors));

	ls_result_t result = LS_E_SUCCESS;

	ls_ripemd160_t ctx;
	ls_ripemd160_digest_t digest;

	struct vector *vec = NULL;
	register size_t i;
	for (i = 0; i < nvectors; ++i) {
		vec = &vectors[i];

		const ls_result_t res = lscst_hash_assert(LS_HASH_RIPEMD160, (const uint8_t *const)vec->data, strlen(vec->data), vec->iterations, vec->digest, digest, LS_RIPEMD160_DIGEST_SIZE);
		if (res != LS_E_SUCCESS) {
			result = LS_E_FAILURE;
			lscst_log(res, meta->name, i, vec->source, vec->digest, digest, LS_RIPEMD160_DIGEST_SIZE);
		}
	}

	ls_memory_clear(&ctx, sizeof(ctx));
	return result;
}




#endif
