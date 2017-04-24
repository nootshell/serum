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

#ifndef __LS_CRYPTO_SYMMETRIC_MODES_CBC_H
#define __LS_CRYPTO_SYMMETRIC_MODES_CBC_H


#include "../../../core/stdincl.h"
#include "../_signatures.h"


#define LS_CBC_PROPAGATE					BIT_1


typedef struct ls_cbc {
	uint8_t *iv;
	uint8_t *cv;
	void *cipher_data;
	ls_sf_encrypt_block cipher_encrypt;
	ls_sf_decrypt_block cipher_decrypt;
	uint16_t block_size;
	uint16_t flags;
} ls_cbc_t;

#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_cbc_init(ls_cbc_t *const cbc, const uint8_t *const iv, const uint16_t block_size, const uint16_t flags, void *const cipher_data, ls_sf_encrypt_block cipher_encrypt, ls_sf_decrypt_block cipher_decrypt);
	LSAPI ls_result_t ls_cbc_clear(ls_cbc_t *cbc);

	LSAPI ls_result_t ls_cbc_reset(const ls_cbc_t *const cbc);

	LSAPI ls_result_t ls_cbc_encrypt(const ls_cbc_t *const LS_RESTRICT cbc, uint8_t *const LS_RESTRICT buffer);
	LSAPI ls_result_t ls_cbc_decrypt(const ls_cbc_t *const LS_RESTRICT cbc, uint8_t *const LS_RESTRICT buffer);

#ifdef __cplusplus
}
#endif


#endif
