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

#define FILE_PATH							"crypto/symmetric/rijndael-cbc.c"

#include "./rijndael-cbc.h"


ID("Rijndael-CBC");


ls_result_t
ls_rijndael_cbc_init(ls_rijndael_cbc_t *const LS_RESTRICT ctx, const void *const LS_RESTRICT key, const size_t key_size, const void *const LS_RESTRICT iv, const uint16_t flags) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	ls_result_t result;

	if (!(result = ls_rijndael_init(&ctx->rijndael, key, key_size)).success) {
		return LS_RESULT_INHERITED(result, false);
	}

	if (!(result = ls_cbc_init(&ctx->cbc, iv, LS_RIJNDAEL_BLOCK_SIZE, flags, &ctx->rijndael, ((ls_sf_encrypt_block)ls_rijndael_encrypt_block), ((ls_sf_decrypt_block)ls_rijndael_decrypt_block))).success) {
		return LS_RESULT_INHERITED(result, false);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_rijndael_cbc_clear(ls_rijndael_cbc_t *const ctx) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	ls_result_t result;

	if (!(result = ls_rijndael_clear(&ctx->rijndael)).success) {
		return LS_RESULT_INHERITED(result, false);
	}

	if (!(result = ls_cbc_clear(&ctx->cbc)).success) {
		return LS_RESULT_INHERITED(result, false);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_rijndael_cbc_reset(const ls_rijndael_cbc_t *const LS_RESTRICT ctx) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	return ls_cbc_reset(&ctx->cbc);
}


ls_result_t
ls_rijndael_cbc_encrypt_block(const ls_rijndael_cbc_t *const LS_RESTRICT ctx, uint32_t *const LS_RESTRICT block) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	return ls_cbc_encrypt_block(&ctx->cbc, (uint8_t *const)block);
}


ls_result_t
ls_rijndael_cbc_decrypt_block(const ls_rijndael_cbc_t *const LS_RESTRICT ctx, uint32_t *const LS_RESTRICT block) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	return ls_cbc_decrypt_block(&ctx->cbc, (uint8_t*)block);
}
