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


#include "./salsa20.h"

#include <string.h>




FILEID("Salsa20 implementation.");




static const char sigma[16] = "expand 32-byte k";
static const char theta[16] = "expand 16-byte k";




ls_result_t
ls_salsa20_init(ls_salsa20_t *const restrict context, const uint8_t *const restrict key, const ls_nword_t key_size, const uint64_t nonce) {
	ls_result_t result;

	result = ls_salsa20_rekey(context, key, key_size);
	if (result != LS_E_SUCCESS) {
		return result;
	}

	result = ls_salsa20_renonce(context, nonce);
	if (result != LS_E_SUCCESS) {
		return result;
	}

	return LS_E_SUCCESS;
}




ls_result_t
ls_salsa20_rekey(ls_salsa20_t *const restrict context, const uint8_t *const restrict key, const ls_nword_t key_size) {
	if (context == NULL || key == NULL) {
		return LS_E_NULL;
	}

	if (key_size != LS_SALSA20_256 && key_size != LS_SALSA20_128) {
		return LS_E_SIZE;
	}

	const uint32_t *key32 = (const uint32_t *)key;
	memcpy(context->layout.k1, key32, sizeof(context->layout.k1));

	{
		const uint32_t *constant = NULL;

		if (key_size == LS_SALSA20_256) {
			constant = (const uint32_t *const)sigma;
			key32 += 4;
		} else if (key_size == LS_SALSA20_128) {
			constant = (const uint32_t *const)theta;
		} else {
			return LS_E_FAILURE;
		}

		context->layout.c1 = LS_ENSURE_LITTLE32(constant[0]);
		context->layout.c2 = LS_ENSURE_LITTLE32(constant[1]);
		context->layout.c3 = LS_ENSURE_LITTLE32(constant[2]);
		context->layout.c4 = LS_ENSURE_LITTLE32(constant[3]);
	}

	memcpy(context->layout.k2, key32, sizeof(context->layout.k2));

	context->layout.counter = 0;
	return LS_E_SUCCESS;
}




ls_result_t
ls_salsa20_renonce(ls_salsa20_t *const context, const uint64_t nonce) {
	if (context == NULL) {
		return LS_E_NULL;
	}

	context->layout.nonce = nonce;

	return LS_E_SUCCESS;
}




ls_result_t
ls_salsa20_block_crypt(ls_salsa20_t *const restrict context, uint8_t block[LS_SALSA20_BLOCK_SIZE]) {
	return LS_E_UNSUPPORTED;
}
