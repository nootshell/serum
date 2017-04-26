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

#define FILE_PATH							"crypto/key_exchange/x25519.c"

#include "./x25519.h"
#include "../../math/curve25519.h"
#include <string.h>


ID("X25519 implementation");


ls_result_t
ls_x25519_init_ex(ls_x25519_t *const ctx, const ls_x25519_key_t private_key, const ls_x25519_key_t basepoint) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(private_key, 2);

	uint8_t param = 0;

	if (!(ctx->private_key = ls_key_alloc_from(private_key, sizeof(ls_x25519_key_t)))) {
		param = 1;
		goto __cleanup;
	}

	if (!(ctx->shared_key = ls_key_alloc(sizeof(ls_x25519_key_t)))) {
		param = 2;
		goto __cleanup;
	}

	if (basepoint) {
		memcpy(ctx->basepoint, basepoint, sizeof(ls_x25519_key_t));
	} else {
		// ctx->basepoint = { 0x09 };
		memset(ctx->basepoint, 0, sizeof(ls_x25519_key_t));
		ctx->basepoint[0] = 0x09;
	}

	ls_curve25519(ctx->public_key, ctx->private_key->data, ctx->basepoint);

	return LS_RESULT_SUCCESS;

__cleanup:
	if (ctx->private_key) {
		ctx->private_key = ls_key_free(ctx->private_key);
	}

	if (ctx->shared_key) {
		ctx->shared_key = ls_key_free(ctx->shared_key);
	}

	return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_ALLOCATION, param);
}


ls_result_t
ls_x25519_init(ls_x25519_t *const ctx, const ls_x25519_key_t private_key) {
	return ls_x25519_init_ex(ctx, private_key, NULL);
}


ls_result_t
ls_x25519_clear(ls_x25519_t *const ctx) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	ctx->private_key = ls_key_free(ctx->private_key);
	ctx->shared_key = ls_key_free(ctx->shared_key);

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_x25519_generate_shared(const ls_x25519_t *const ctx, const ls_x25519_key_t public_key) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(public_key, 2);
	LS_RESULT_CHECK_NULL(ctx->private_key, 3);
	LS_RESULT_CHECK_NULL(ctx->shared_key, 4);

	ls_curve25519(ctx->shared_key->data, ctx->private_key->data, public_key);

	return LS_RESULT_SUCCESS;
}


const void*
ls_x25519_get_public(const ls_x25519_t *const ctx) {
	if (!ctx) {
		return NULL;
	}

	return ctx->public_key;
}


const void*
ls_x25519_get_private(const ls_x25519_t *const ctx) {
	if (!ctx || !ctx->private_key) {
		return NULL;
	}

	return ctx->private_key->data;
}


const void*
ls_x25519_get_shared(const ls_x25519_t *const ctx) {
	if (!ctx || !ctx->shared_key) {
		return NULL;
	}

	return ctx->shared_key->data;
}
