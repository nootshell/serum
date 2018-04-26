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


#ifndef __LS_CRYPTO_CIPHERS_SALSA20_H
#define __LS_CRYPTO_CIPHERS_SALSA20_H




#include "../../core/setup.h"




#define LS_SALSA20_BLOCK_SIZE				64




typedef union ls_salsa20 {
	struct {
		uint32_t c1;
		uint32_t k1[4];
		uint32_t c2;
		uint64_t nonce;
		uint64_t counter;
		uint32_t c3;
		uint32_t k2[4];
		uint32_t c4;
	} layout;
	uint32_t words[16];
} ls_salsa20_t;




#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_salsa20_init(ls_salsa20_t *const restrict context, const uint8_t *const restrict key, const ls_nword_t key_size, const uint64_t nonce);
	LSAPI ls_result_t ls_salsa20_rekey(ls_salsa20_t *const restrict context, const uint8_t *const restrict key, const ls_nword_t key_size);
	LSAPI ls_result_t ls_salsa20_renonce(ls_salsa20_t *const context, const uint64_t nonce);

	LSAPI ls_result_t ls_salsa20_block_crypt(ls_salsa20_t *const restrict context, uint8_t block[LS_SALSA20_BLOCK_SIZE]);

#ifdef __cplusplus
}
#endif




#endif
