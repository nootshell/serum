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


#include "./md5.h"

#include "../../../../core/memory.h"
#include "../../base.h"

#include <stdio.h>
#include <string.h>




FILEID("CST: MD5 [pure]");




struct vector {
	ls_md5_digest_t digest;
	char data[LS_MD5_BLOCK_SIZE];
	size_t iterations;
	char source[32];
} vectors[] = {
	{
		.digest = { 0xD4, 0x1D, 0x8C, 0xD9, 0x8F, 0x00, 0xB2, 0x04, 0xE9, 0x80, 0x09, 0x98, 0xEC, 0xF8, 0x42, 0x7E },
		.data = "",
		.iterations = 1,
		.source = "RFC 1321 / IST-1999-12324"
	},
	{
		.digest = { 0x0C, 0xC1, 0x75, 0xB9, 0xC0, 0xF1, 0xB6, 0xA8, 0x31, 0xC3, 0x99, 0xE2, 0x69, 0x77, 0x26, 0x61 },
		.data = "a",
		.iterations = 1,
		.source = "RFC 1321 / IST-1999-12324"
	},
	{
		.digest = { 0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0, 0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72 },
		.data = "abc",
		.iterations = 1,
		.source = "RFC 1321 / IST-1999-12324"
	},
	{
		.digest = { 0xF9, 0x6B, 0x69, 0x7D, 0x7C, 0xB7, 0x93, 0x8D, 0x52, 0x5A, 0x2F, 0x31, 0xAA, 0xF1, 0x61, 0xD0 },
		.data = "message digest",
		.iterations = 1,
		.source = "RFC 1321 / IST-1999-12324"
	},
	{
		.digest = { 0xC3, 0xFC, 0xD3, 0xD7, 0x61, 0x92, 0xE4, 0x00, 0x7D, 0xFB, 0x49, 0x6C, 0xCA, 0x67, 0xE1, 0x3B },
		.data = "abcdefghijklmnopqrstuvwxyz",
		.iterations = 1,
		.source = "RFC 1321 / IST-1999-12324"
	},
	{
		.digest = { 0xD1, 0x74, 0xAB, 0x98, 0xD2, 0x77, 0xD9, 0xF5, 0xA5, 0x61, 0x1C, 0x2C, 0x9F, 0x41, 0x9D, 0x9F },
		.data = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		.iterations = 1,
		.source = "RFC 1321"
	},
	{
		.digest = { 0x82, 0x15, 0xEF, 0x07, 0x96, 0xA2, 0x0B, 0xCA, 0xAA, 0xE1, 0x16, 0xD3, 0x87, 0x6C, 0x66, 0x4A },
		.data = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
		.iterations = 1,
		.source = "IST-1999-12324 / NSRL Test Data"
	},
	{
		.digest = { 0x9E, 0x10, 0x7D, 0x9D, 0x37, 0x2B, 0xB6, 0x82, 0x6B, 0xD8, 0x1D, 0x35, 0x42, 0xA4, 0x19, 0xD6 },
		.data = "The quick brown fox jumps over the lazy dog",
		.iterations = 1,
		.source = "Wikipedia"
	},
	{
		.digest = { 0xE4, 0xD9, 0x09, 0xC2, 0x90, 0xD0, 0xFB, 0x1C, 0xA0, 0x68, 0xFF, 0xAD, 0xDF, 0x22, 0xCB, 0xD0 },
		.data = "The quick brown fox jumps over the lazy dog.",
		.iterations = 1,
		.source = "Wikipedia"
	},
	{
		.digest = { 0x57, 0xED, 0xF4, 0xA2, 0x2B, 0xE3, 0xC9, 0x55, 0xAC, 0x49, 0xDA, 0x2E, 0x21, 0x07, 0xB6, 0x7A },
		.data = "1234567890",
		.iterations = 8,
		.source = "RFC 1321 / IST-1999-12324"
	},
	{
		.digest = { 0x77, 0x07, 0xD6, 0xAE, 0x4E, 0x02, 0x7C, 0x70, 0xEE, 0xA2, 0xA9, 0x35, 0xC2, 0x29, 0x6F, 0x21 },
		.data = "a",
		.iterations = 1000000,
		.source = "IST-1999-12324 / NSRL Test Data"
	}
};




ls_result_t
lscst_hashing_md5min(const lsreg_meta_t *const meta) {
	const size_t nvectors = (sizeof(vectors) / sizeof(*vectors));

	ls_result_t result = LS_E_SUCCESS;

	lsp_md5_data_t ctx;
	ls_md5_digest_t digest;

	size_t len = 0;
	struct vector *vec = NULL;
	register size_t i;
	for (i = 0; i < nvectors; ++i) {
		vec = &vectors[i];

		if (vec->iterations > 1) {
			// TODO!
			continue;
		}

		if (lsp_md5_init(&ctx) != LS_E_SUCCESS) {
			lscst_log(LS_E_INITIALIZATION, meta->name, i, vec->source, NULL, NULL, 0);
			continue;
		}

		len = strlen(vec->data);
		if (lsp_md5_finish(&ctx, (const uint8_t *const)vec->data, len, (len * LS_BITS_BYTE), digest) != LS_E_SUCCESS) {
			lscst_log(LS_E_CONVERSION, meta->name, i, vec->source, NULL, NULL, 0);
			continue;
		}

		if (memcmp(vec->digest, digest, sizeof(digest)) != 0) {
			lscst_log(LS_E_DATA_MISMATCH, meta->name, i, vec->source, vec->digest, digest, sizeof(digest));
			result = LS_E_FAILURE;
			continue;
		}
	}

	ls_memory_clear(&ctx, sizeof(ctx));
	return result;
}
