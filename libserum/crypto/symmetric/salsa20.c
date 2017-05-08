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

#define FILE_PATH							"crypto/symmetric/salsa20.c"

#include "./salsa20.h"
#include "../../core/memory.h"
#include <string.h>


ID("Salsa20 implementation");


static const uint8_t sigma[16] = "expand 32-byte k";
static const uint8_t theta[16] = "expand 16-byte k";


void
static ls_salsa20_get_block(ls_salsa20_t *ctx) {
	uint32_t d[16];
	memcpy(d, ctx, sizeof(d));

	ls_nword_t i;

	for (i = 10; i--;) {
		d[ 4] ^= LS_ROTL32((d[ 0] + d[12]), 0x07);
		d[ 8] ^= LS_ROTL32((d[ 4] + d[ 0]), 0x09);
		d[12] ^= LS_ROTL32((d[ 8] + d[ 4]), 0x0D);
		d[ 0] ^= LS_ROTL32((d[12] + d[ 8]), 0x12);
		d[ 9] ^= LS_ROTL32((d[ 5] + d[ 1]), 0x07);
		d[13] ^= LS_ROTL32((d[ 9] + d[ 5]), 0x09);
		d[ 1] ^= LS_ROTL32((d[13] + d[ 9]), 0x0D);
		d[ 5] ^= LS_ROTL32((d[ 1] + d[13]), 0x12);
		d[14] ^= LS_ROTL32((d[10] + d[ 6]), 0x07);
		d[ 2] ^= LS_ROTL32((d[14] + d[10]), 0x09);
		d[ 6] ^= LS_ROTL32((d[ 2] + d[14]), 0x0D);
		d[10] ^= LS_ROTL32((d[ 6] + d[ 2]), 0x12);
		d[ 3] ^= LS_ROTL32((d[15] + d[11]), 0x07);
		d[ 7] ^= LS_ROTL32((d[ 3] + d[15]), 0x09);
		d[11] ^= LS_ROTL32((d[ 7] + d[ 3]), 0x0D);
		d[15] ^= LS_ROTL32((d[11] + d[ 7]), 0x12);
		d[ 1] ^= LS_ROTL32((d[ 0] + d[ 3]), 0x07);
		d[ 2] ^= LS_ROTL32((d[ 1] + d[ 0]), 0x09);
		d[ 3] ^= LS_ROTL32((d[ 2] + d[ 1]), 0x0D);
		d[ 0] ^= LS_ROTL32((d[ 3] + d[ 2]), 0x12);
		d[ 6] ^= LS_ROTL32((d[ 5] + d[ 4]), 0x07);
		d[ 7] ^= LS_ROTL32((d[ 6] + d[ 5]), 0x09);
		d[ 4] ^= LS_ROTL32((d[ 7] + d[ 6]), 0x0D);
		d[ 5] ^= LS_ROTL32((d[ 4] + d[ 7]), 0x12);
		d[11] ^= LS_ROTL32((d[10] + d[ 9]), 0x07);
		d[ 8] ^= LS_ROTL32((d[11] + d[10]), 0x09);
		d[ 9] ^= LS_ROTL32((d[ 8] + d[11]), 0x0D);
		d[10] ^= LS_ROTL32((d[ 9] + d[ 8]), 0x12);
		d[12] ^= LS_ROTL32((d[15] + d[14]), 0x07);
		d[13] ^= LS_ROTL32((d[12] + d[15]), 0x09);
		d[14] ^= LS_ROTL32((d[13] + d[12]), 0x0D);
		d[15] ^= LS_ROTL32((d[14] + d[13]), 0x12);
	}

	for (i = 16; i--;) {
		((uint32_t*)ctx->cache)[i] = (d[i] + ctx->data.data[i]);
	}

	if (!++(ctx->data.data[8])) {
		if (!++(ctx->data.data[9])) {

		}
	}

	ctx->cache_offset = 0;
}


void
static LS_ATTR_INLINE ls_salsa20_internal_process_sub(ls_salsa20_t *const LS_RESTRICT ctx, uint8_t *const LS_RESTRICT data, const size_t size) {
	ls_nword_t i = 0;

	for (; ctx->cache_offset < LS_SALSA20_BLOCK_SIZE;) {
		data[i++] ^= ctx->cache[ctx->cache_offset++];
	}

	ls_salsa20_get_block(ctx);

	for (; i < size;) {
		data[i++] ^= ctx->cache[ctx->cache_offset++];
	}
}


ls_result_t
ls_salsa20_init_ex(ls_salsa20_t *const LS_RESTRICT ctx, const void *const LS_RESTRICT key, const size_t key_size, const void *const LS_RESTRICT iv) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(key, 2);

#if (LS_SALSA20_STRICT)
	LS_RESULT_CHECK((key_size != 16 && key_size != 32), LS_RESULT_CODE_SIZE, 1);
#else
	LS_RESULT_CHECK((key_size < 16), LS_RESULT_CODE_SIZE, 1);
#endif


	if (!LS_MEMLOCK(&ctx->data, sizeof(ctx->data))) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_LOCK);
	}


	const uint32_t
		*c = NULL,
		*k32 = key;

	memcpy(ctx->data.layout.k1, k32, sizeof(ctx->data.layout.k1));

	if (key_size >= 32) {
		k32 += 4;
		c = (const uint32_t*)sigma;
	} else if (key_size >= 16) {
		c = (const uint32_t*)theta;
	}

	memcpy(ctx->data.layout.k2, k32, sizeof(ctx->data.layout.k2));

	ctx->data.layout.c1 = LS_SWAP_ENDIAN_LITTLE_32(c[0]);
	ctx->data.layout.c2 = LS_SWAP_ENDIAN_LITTLE_32(c[1]);
	ctx->data.layout.c3 = LS_SWAP_ENDIAN_LITTLE_32(c[2]);
	ctx->data.layout.c4 = LS_SWAP_ENDIAN_LITTLE_32(c[3]);

	if (iv) {
		memcpy(&ctx->data.layout.nonce, iv, sizeof(ctx->data.layout.nonce));
	} else {
		ctx->data.layout.nonce = 0;
	}

	ctx->cache_offset = LS_SALSA20_BLOCK_SIZE;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_salsa20_init(ls_salsa20_t *const LS_RESTRICT ctx, const void *const LS_RESTRICT key, const size_t key_size) {
	return ls_salsa20_init_ex(ctx, key, key_size, NULL);
}


ls_result_t
ls_salsa20_clear(ls_salsa20_t *ctx) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	if (!LS_MEMUNLOCK(&ctx->data, sizeof(ctx->data))) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_LOCK);
	}

	memset(ctx, 0, sizeof(*ctx));

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_salsa20_process_block(ls_salsa20_t *const LS_RESTRICT ctx, void *const LS_RESTRICT data) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(data, 2);

	ls_salsa20_internal_process_sub(ctx, data, LS_SALSA20_BLOCK_SIZE);

	return LS_RESULT_SUCCESS;
}

ls_result_t
ls_salsa20_encrypt_block(ls_salsa20_t *const LS_RESTRICT ctx, void *const LS_RESTRICT data) {
	return ls_salsa20_process_block(ctx, data);
}

ls_result_t
ls_salsa20_decrypt_block(ls_salsa20_t *const LS_RESTRICT ctx, void *const LS_RESTRICT data) {
	return ls_salsa20_process_block(ctx, data);
}


ls_result_t
ls_salsa20_process_data(ls_salsa20_t *const LS_RESTRICT ctx, void *const LS_RESTRICT data, size_t size) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(data, 2);
	LS_RESULT_CHECK_SIZE(size, 1);

	ctx->data.layout.counter = 0;

	uint8_t *data8 = data;

	for (; size >= LS_SALSA20_BLOCK_SIZE;) {
		ls_salsa20_process_block(ctx, data8);
		data8 += LS_SALSA20_BLOCK_SIZE;
		size -= LS_SALSA20_BLOCK_SIZE;
	}

	if (size) {

		if ((LS_SALSA20_BLOCK_SIZE - ctx->cache_offset) >= size) {
		ls_nword_t i = 0;
			for (; i < size;) {
				data8[i++] ^= ctx->cache[ctx->cache_offset++];
			}
		} else {
			ls_salsa20_internal_process_sub(ctx, data8, size);
		}
	}

	return LS_RESULT_SUCCESS;
}

ls_result_t
ls_salsa20_encrypt(ls_salsa20_t *const LS_RESTRICT ctx, void *const LS_RESTRICT data, size_t size) {
	return ls_salsa20_process_data(ctx, data, size);
}

ls_result_t
ls_salsa20_decrypt(ls_salsa20_t *const LS_RESTRICT ctx, void *const LS_RESTRICT data, size_t size) {
	return ls_salsa20_process_data(ctx, data, size);
}
