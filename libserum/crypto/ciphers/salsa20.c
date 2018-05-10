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
ls_salsa20_init(ls_salsa20_t *const restrict context, const uint8_t *const restrict key, const ls_nword_t key_size, const uint64_t *const restrict nonce) {
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
ls_salsa20_renonce(ls_salsa20_t *const restrict context, const uint64_t *const restrict nonce) {
	if (context == NULL) {
		return LS_E_NULL;
	}

	context->layout.counter = 0;
	context->layout.nonce = ((nonce == NULL) ? 0 : *nonce);

	return LS_E_SUCCESS;
}




ls_result_t
ls_salsa20_get_stream_block_ex(ls_salsa20_t *const restrict context, ls_salsa20_block_t block, const ls_nword_t rounds) {
	if (context == NULL || block == NULL) {
		return LS_E_NULL;
	}

	if ((rounds & 1) != 0) {
		return LS_E_SIZE;
	}


	uint32_t buffer[16];
	memcpy(buffer, context->words, sizeof(buffer));

	ls_nword_t i;
	for (i = (rounds >> 1); i--;) {
		buffer[ 4] ^= LS_ROTL32((buffer[ 0] + buffer[12]), 0x07);
		buffer[ 8] ^= LS_ROTL32((buffer[ 4] + buffer[ 0]), 0x09);
		buffer[12] ^= LS_ROTL32((buffer[ 8] + buffer[ 4]), 0x0D);
		buffer[ 0] ^= LS_ROTL32((buffer[12] + buffer[ 8]), 0x12);

		buffer[ 9] ^= LS_ROTL32((buffer[ 5] + buffer[ 1]), 0x07);
		buffer[13] ^= LS_ROTL32((buffer[ 9] + buffer[ 5]), 0x09);
		buffer[ 1] ^= LS_ROTL32((buffer[13] + buffer[ 9]), 0x0D);
		buffer[ 5] ^= LS_ROTL32((buffer[ 1] + buffer[13]), 0x12);

		buffer[14] ^= LS_ROTL32((buffer[10] + buffer[ 6]), 0x07);
		buffer[ 2] ^= LS_ROTL32((buffer[14] + buffer[10]), 0x09);
		buffer[ 6] ^= LS_ROTL32((buffer[ 2] + buffer[14]), 0x0D);
		buffer[10] ^= LS_ROTL32((buffer[ 6] + buffer[ 2]), 0x12);

		buffer[ 3] ^= LS_ROTL32((buffer[15] + buffer[11]), 0x07);
		buffer[ 7] ^= LS_ROTL32((buffer[ 3] + buffer[15]), 0x09);
		buffer[11] ^= LS_ROTL32((buffer[ 7] + buffer[ 3]), 0x0D);
		buffer[15] ^= LS_ROTL32((buffer[11] + buffer[ 7]), 0x12);

		buffer[ 1] ^= LS_ROTL32((buffer[ 0] + buffer[ 3]), 0x07);
		buffer[ 2] ^= LS_ROTL32((buffer[ 1] + buffer[ 0]), 0x09);
		buffer[ 3] ^= LS_ROTL32((buffer[ 2] + buffer[ 1]), 0x0D);
		buffer[ 0] ^= LS_ROTL32((buffer[ 3] + buffer[ 2]), 0x12);

		buffer[ 6] ^= LS_ROTL32((buffer[ 5] + buffer[ 4]), 0x07);
		buffer[ 7] ^= LS_ROTL32((buffer[ 6] + buffer[ 5]), 0x09);
		buffer[ 4] ^= LS_ROTL32((buffer[ 7] + buffer[ 6]), 0x0D);
		buffer[ 5] ^= LS_ROTL32((buffer[ 4] + buffer[ 7]), 0x12);

		buffer[11] ^= LS_ROTL32((buffer[10] + buffer[ 9]), 0x07);
		buffer[ 8] ^= LS_ROTL32((buffer[11] + buffer[10]), 0x09);
		buffer[ 9] ^= LS_ROTL32((buffer[ 8] + buffer[11]), 0x0D);
		buffer[10] ^= LS_ROTL32((buffer[ 9] + buffer[ 8]), 0x12);

		buffer[12] ^= LS_ROTL32((buffer[15] + buffer[14]), 0x07);
		buffer[13] ^= LS_ROTL32((buffer[12] + buffer[15]), 0x09);
		buffer[14] ^= LS_ROTL32((buffer[13] + buffer[12]), 0x0D);
		buffer[15] ^= LS_ROTL32((buffer[14] + buffer[13]), 0x12);
	}

	uint32_t *const block32 = (uint32_t *const)block;
	for (i = (sizeof(context->words) / sizeof(*context->words)); i--;) {
		block32[i] = (buffer[i] + context->words[i]);
	}


	if (++context->words[8] == 0) {
		if (++context->words[9] == 0) {
			return LS_E_DEPLETED;
		}
	}


	return LS_E_SUCCESS;
}


ls_result_t
ls_salsa20_get_stream_block(ls_salsa20_t *const restrict context, ls_salsa20_block_t block) {
	return ls_salsa20_get_stream_block_ex(context, block, LS_SALSA20_ROUNDS);
}




ls_result_t
ls_salsa20_block_crypt(ls_salsa20_t *const restrict context, ls_salsa20_block_t block) {
	if (context == NULL || block == NULL) {
		return LS_E_NULL;
	}


	uint32_t buffer[(LS_SALSA20_BLOCK_SIZE / sizeof(uint32_t))];
	const ls_result_t block_result = ls_salsa20_get_stream_block(context, (uint8_t *const)buffer);
	if (block_result != LS_E_SUCCESS && block_result != LS_E_DEPLETED) {
		return block_result;
	}


	uint32_t *const block32 = (uint32_t *const)block;

	ls_nword_t i;
	for (i = (sizeof(buffer) / sizeof(*buffer)); i--;) {
		block32[i] ^= buffer[i];
	}


	return block_result;
}
