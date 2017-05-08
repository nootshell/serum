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

#ifndef __LS_CRYPTO_SYMMETRIC_SALSA_20_H
#define __LS_CRYPTO_SYMMETRIC_SALSA_20_H


#include "../../core/stdincl.h"


#define LS_SALSA20_BLOCK_SIZE				64


struct ls_salsa20_layout {
	uint32_t c1;
	uint32_t k1[4];
	uint32_t c2;
	uint64_t nonce;
	uint64_t counter;
	uint32_t c3;
	uint32_t k2[4];
	uint32_t c4;
};

union ls_salsa20_data {
	uint32_t data[16];
	struct ls_salsa20_layout layout;
};

typedef struct ls_salsa20 {
	union ls_salsa20_data data;
	uint8_t cache[LS_SALSA20_BLOCK_SIZE];
	ls_nword_t cache_offset;
} ls_salsa20_t;


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_salsa20_init_ex(ls_salsa20_t *const LS_RESTRICT ctx, const void *const LS_RESTRICT key, const size_t key_size, const void *const LS_RESTRICT iv);
	LSAPI ls_result_t ls_salsa20_init(ls_salsa20_t *const LS_RESTRICT ctx, const void *const LS_RESTRICT key, const size_t key_size);
	LSAPI ls_result_t ls_salsa20_clear(ls_salsa20_t *ctx);

	LSAPI ls_result_t ls_salsa20_process_block(ls_salsa20_t *const LS_RESTRICT ctx, void *const LS_RESTRICT data);
	LSAPI ls_result_t ls_salsa20_encrypt_block(ls_salsa20_t *const LS_RESTRICT ctx, void *const LS_RESTRICT data);
	LSAPI ls_result_t ls_salsa20_decrypt_block(ls_salsa20_t *const LS_RESTRICT ctx, void *const LS_RESTRICT data);

	LSAPI ls_result_t ls_salsa20_process_data(ls_salsa20_t *const LS_RESTRICT ctx, void *const LS_RESTRICT data, size_t size);
	LSAPI ls_result_t ls_salsa20_encrypt(ls_salsa20_t *const LS_RESTRICT ctx, void *const LS_RESTRICT data, size_t size);
	LSAPI ls_result_t ls_salsa20_decrypt(ls_salsa20_t *const LS_RESTRICT ctx, void *const LS_RESTRICT data, size_t size);

#ifdef __cplusplus
}
#endif


#endif
