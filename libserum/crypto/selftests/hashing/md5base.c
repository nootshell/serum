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


#include "./md5base.h"

#include "../base.h"

#include <stdio.h>
#include <string.h>




FILEID("MD5 (base) selftests.");




struct vector {
	uint8_t digest[16];
	char data[64];
} vectors[] = {
	{
		.data = "",
		.digest = { 0xD4, 0x1D, 0x8C, 0xD9, 0x8F, 0x00, 0xB2, 0x04, 0xE9, 0x80, 0x09, 0x98, 0xEC, 0xF8, 0x42, 0x7E }
	},
	{
		.data = "a",
		.digest = { 0x0C, 0xC1, 0x75, 0xB9, 0xC0, 0xF1, 0xB6, 0xA8, 0x31, 0xC3, 0x99, 0xE2, 0x69, 0x77, 0x26, 0x61 }
	},
	{
		.data = "abc",
		.digest = { 0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0, 0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72 }
	},
	{
		.data = "message digest",
		.digest = { 0xF9, 0x6B, 0x69, 0x7D, 0x7C, 0xB7, 0x93, 0x8D, 0x52, 0x5A, 0x2F, 0x31, 0xAA, 0xF1, 0x61, 0xD0 }
	},
	{
		.data = "abcdefghijklmnopqrstuvwxyz",
		.digest = { 0xC3, 0xFC, 0xD3, 0xD7, 0x61, 0x92, 0xE4, 0x00, 0x7D, 0xFB, 0x49, 0x6C, 0xCA, 0x67, 0xE1, 0x3B }
	},
	{
		.data = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
		.digest = { 0x82, 0x15, 0xEF, 0x07, 0x96, 0xA2, 0x0B, 0xCA, 0xAA, 0xE1, 0x16, 0xD3, 0x87, 0x6C, 0x66, 0x4A }
	},
	{
		.data = "The quick brown fox jumps over the lazy dog",
		.digest = { 0x9E, 0x10, 0x7D, 0x9D, 0x37, 0x2B, 0xB6, 0x82, 0x6B, 0xD8, 0x1D, 0x35, 0x42, 0xA4, 0x19, 0xD6 }
	},
	{
		.data = "The quick brown fox jumps over the lazy dog.",
		.digest = { 0xE4, 0xD9, 0x09, 0xC2, 0x90, 0xD0, 0xFB, 0x1C, 0xA0, 0x68, 0xFF, 0xAD, 0xDF, 0x22, 0xCB, 0xD0 }
	}
};




ls_result_t
lscst_hashing_md5min(void *const __st) {
	const size_t n = (sizeof(vectors) / sizeof(*vectors));

	ls_result_t result = LS_E_SUCCESS;

	ls_md5base_data_t ctx;
	ls_md5_digest_t digest;

	size_t i, len;
	struct vector *vec;
	for (i = 0; i < n; ++i) {
		if (ls_md5base_init(&ctx) != LS_E_SUCCESS) {
			lscst_report_failure(__st, "Failed to initialize context.");
			result = LS_E_FAILURE;
			continue;
		}

		vec = &vectors[i];

		len = strlen(vec->data);
		if (ls_md5base_finish(&ctx, (const uint8_t *const)vec->data, len, (len * LS_BITS_BYTE), digest) != LS_E_SUCCESS) {
			lscst_report_failure(__st, "Failed to finish context.");
			result = LS_E_FAILURE;
			continue;
		}

		if (memcmp(vec->digest, digest, sizeof(digest)) != 0) {
			lscst_report_failure(__st, "Digest mismatch.");
			result = LS_E_FAILURE;
			continue;
		}
	}

	memset(&ctx, 0, sizeof(ctx));
	return result;
}
