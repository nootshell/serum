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


ID("Cipher Block Chaining");


ls_result_t
ls_cbc_init(ls_cbc_t *cbc, size_t block_size, void *cipher_data, ls_result_t(*cipher_encrypt)(void *data, void *block), ls_result_t(*cipher_decrypt)(void *data, void *block)) {
	LS_RESULT_CHECK_NULL(cbc, 1);

	memset(cbc->iv, 0xBA, sizeof(cbc->iv));
	memcpy(cbc->xor, cbc->iv, sizeof(cbc->iv));
	cbc->block_size = block_size;
	cbc->cipher_data = cipher_data;
	cbc->cipher_encrypt = cipher_encrypt;
	cbc->cipher_decrypt = cipher_decrypt;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_cbc_encrypt(ls_cbc_t *cbc, uint8_t *block) {
	LS_RESULT_CHECK_NULL(cbc, 1);
	LS_RESULT_CHECK_NULL(cbc->cipher_data, 2);

	unsigned int i;
	for (i = cbc->block_size; i--;) {
		block[i] = (block[i] ^ cbc->xor[i]);
	}

	if (!(cbc->cipher_encrypt(cbc->cipher_data, block).success)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	memcpy(cbc->xor, block, cbc->block_size);

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_cbc_decrypt(ls_cbc_t *cbc, uint8_t *block) {
	LS_RESULT_CHECK_NULL(cbc, 1);
	LS_RESULT_CHECK_NULL(cbc->cipher_data, 2);

	uint8_t *ct_old = malloc(cbc->block_size);
	memcpy(ct_old, block, cbc->block_size);

	if (!(cbc->cipher_decrypt(cbc->cipher_data, block).success)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	unsigned int i;
	for (i = cbc->block_size; i--;) {
		block[i] = (block[i] ^ cbc->xor[i]);
	}

	memcpy(cbc->xor, ct_old, cbc->block_size);
	free(ct_old);

	return LS_RESULT_SUCCESS;
}

ls_result_t
ls_cbc_encrypt2(ls_cbc_t *cbc, uint8_t *block) {
	LS_RESULT_CHECK_NULL(cbc, 1);
	LS_RESULT_CHECK_NULL(cbc->cipher_data, 2);

	unsigned int i;
	for (i = cbc->block_size; i--;) {
		block[i] = (block[i] ^ cbc->xor[i]);
	}

	if (!(cbc->cipher_encrypt(cbc->cipher_data, block).success)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	memcpy(cbc->xor, block, cbc->block_size);

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_cbc_decrypt2(ls_cbc_t *cbc, uint8_t *block) {
	LS_RESULT_CHECK_NULL(cbc, 1);
	LS_RESULT_CHECK_NULL(cbc->cipher_data, 2);

	uint8_t *ct_old = malloc(cbc->block_size);
	memcpy(ct_old, block, cbc->block_size);

	if (!(cbc->cipher_decrypt(cbc->cipher_data, block).success)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	unsigned int i;
	for (i = cbc->block_size; i--;) {
		block[i] = (block[i] ^ cbc->xor[i]);
	}

	memcpy(cbc->xor, ct_old, cbc->block_size);
	free(ct_old);

	return LS_RESULT_SUCCESS;
}

