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
**    Based on implementation found at https://en.wikipedia.org/wiki/XXTEA
**
*/

#define FILE_PATH							"crypto/symmetric/xxtea.c"

#include "./xxtea.h"
#include "../../core/ptrarithmetic.h"
#include <string.h>


ID("XXTEA implementation");


#define XXTEA_ROUND(key, a, b, e, p, sum)	((((b >> 5) ^ (a << 2)) + ((a >> 3) ^ (b << 4))) ^ ((sum ^ a) + (key[(p & 3) ^ e] ^ b)))


ls_result_t
ls_xxtea_init(ls_xxtea_t *const LS_RESTRICT ctx, const void *const LS_RESTRICT key) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(key, 2);

	ctx->key = ls_key_alloc_from(key, 16);

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_xxtea_clear(ls_xxtea_t *const ctx) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	if (ctx->key) {
		ctx->key = ls_key_free(ctx->key);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_xxtea_encrypt_block(const ls_xxtea_t *const LS_RESTRICT ctx, uint32_t *const LS_RESTRICT block, const size_t block_count) {
	size_t p;
	const size_t nm1 = (block_count - 1);
	register uint32_t a, b, sum;
	const uint32_t *const key = ((uint32_t*)ctx->key->data);
	uint_fast16_t e, rounds;

	p = a = b = e = 0;
	rounds = (uint_fast16_t)(6 + (52U / block_count));
	sum = 0;
	b = block[nm1];

	do {
		sum += 0x9E3779B9;
		e = ((sum >> 2) & 3);
		for (p = 0; p < nm1; ++p) {
			a = block[p + 1];
			b = (block[p] += XXTEA_ROUND(key, a, b, e, p, sum));
		}
		a = block[0];
		b = (block[nm1] += XXTEA_ROUND(key, a, b, e, p, sum));
	} while (--rounds);

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_xxtea_decrypt_block(const ls_xxtea_t *const LS_RESTRICT ctx, uint32_t *const LS_RESTRICT block, const size_t block_count) {
	size_t p;
	const size_t nm1 = (block_count - 1);
	register uint32_t a, b, sum;
	const uint32_t *const key = ((uint32_t*)ctx->key->data);
	uint_fast16_t e, rounds;

	p = a = b = e = 0;
	rounds = (uint_fast16_t)(6 + (52U / block_count));
	sum = (rounds * 0x9E3779B9);
	a = block[0];

	do {
		e = ((sum >> 2) & 3);
		for (p = block_count; --p > 0;) {
			b = block[p - 1];
			a = (block[p] -= XXTEA_ROUND(key, a, b, e, p, sum));
		}
		b = block[nm1];
		a = (block[0] -= XXTEA_ROUND(key, a, b, e, p, sum));
		sum -= 0x9E3779B9;
	} while (--rounds);

	return LS_RESULT_SUCCESS;
}
