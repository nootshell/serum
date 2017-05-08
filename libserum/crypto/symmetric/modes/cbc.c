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

#define FILE_PATH							"crypto/symmetric/modes/cbc.c"

#include "./cbc.h"
#include "../../../core/memory.h"
#include <string.h>


ID("Cipher Block Chaining");


ls_result_t
ls_cbc_init(ls_cbc_t *const cbc, const uint8_t *const iv, const uint16_t block_size, const uint16_t flags, void *const data, ls_sym_encrypt_block_func_t encrypt, ls_sym_decrypt_block_func_t decrypt) {
	LS_RESULT_CHECK_NULL(cbc, 1);
	LS_RESULT_CHECK_NULL(iv, 2);
	LS_RESULT_CHECK_NULL(data, 3);
	LS_RESULT_CHECK_NULL(encrypt, 4);
	LS_RESULT_CHECK_NULL(decrypt, 5);

	cbc->block_size = block_size;
	cbc->iv = malloc(cbc->block_size);
	cbc->cv = malloc(cbc->block_size);
	cbc->data = data;
	cbc->encrypt = encrypt;
	cbc->decrypt = decrypt;

	cbc->flags = flags;

	memcpy(cbc->iv, iv, cbc->block_size);
	memcpy(cbc->cv, cbc->iv, cbc->block_size);

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_cbc_clear(ls_cbc_t *cbc) {
	LS_RESULT_CHECK_NULL(cbc, 1);

	if (cbc->iv) {
		free(cbc->iv);
	}

	if (cbc->cv) {
		free(cbc->cv);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_cbc_reset(const ls_cbc_t *const cbc) {
	LS_RESULT_CHECK_NULL(cbc, 1);
	LS_RESULT_CHECK_NULL(cbc->iv, 1);
	LS_RESULT_CHECK_NULL(cbc->cv, 1);

	memcpy(cbc->cv, cbc->iv, cbc->block_size);

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_cbc_encrypt_block(const ls_cbc_t *const LS_RESTRICT cbc, uint8_t *const LS_RESTRICT buffer) {
	LS_RESULT_CHECK_NULL(cbc, 1);
	LS_RESULT_CHECK_NULL(cbc->data, 2);

	uint8_t *pt_old = NULL;
	if (HAS_FLAG(cbc->flags, LS_CBC_PROPAGATE)) {
		pt_old = malloc(cbc->block_size);
		memcpy(pt_old, buffer, cbc->block_size);
	}

	ls_nword_t i;
	for (i = cbc->block_size; i--;) {
		buffer[i] = (buffer[i] ^ cbc->cv[i]);
	}

	if (!(cbc->encrypt(cbc->data, buffer).success)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	memcpy(cbc->cv, buffer, cbc->block_size);

	if (HAS_FLAG(cbc->flags, LS_CBC_PROPAGATE)) {
		for (i = cbc->block_size; i--;) {
			cbc->cv[i] = (cbc->cv[i] ^ pt_old[i]);
		}
		free(pt_old);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_cbc_decrypt_block(const ls_cbc_t *const LS_RESTRICT cbc, uint8_t *const LS_RESTRICT buffer) {
	LS_RESULT_CHECK_NULL(cbc, 1);
	LS_RESULT_CHECK_NULL(cbc->data, 2);

	uint8_t *ct_old = malloc(cbc->block_size);
	memcpy(ct_old, buffer, cbc->block_size);

	if (!(cbc->decrypt(cbc->data, buffer).success)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	ls_nword_t i;
	if (HAS_FLAG(cbc->flags, LS_CBC_PROPAGATE)) {
		for (i = cbc->block_size; i--;) {
			buffer[i] = (buffer[i] ^ cbc->cv[i]);
			ct_old[i] = (buffer[i] ^ ct_old[i]);
		}
	} else {
		for (i = cbc->block_size; i--;) {
			buffer[i] = (buffer[i] ^ cbc->cv[i]);
		}
	}

	memcpy(cbc->cv, ct_old, cbc->block_size);
	free(ct_old);

	return LS_RESULT_SUCCESS;
}
