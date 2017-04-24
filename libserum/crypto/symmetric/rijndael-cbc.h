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

#ifndef __LS_CRYPTO_SYMMETRIC_RIJNDAEL_CBC_H
#define __LS_CRYPTO_SYMMETRIC_RIJNDAEL_CBC_H


#include "../../core/stdincl.h"
#include "./rijndael.h"
#include "./modes/cbc.h"


typedef struct ls_rijndael_cbc {
	ls_rijndael_t rijndael;
	ls_cbc_t cbc;
} ls_rijndael_cbc_t;


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_rijndael_cbc_init(ls_rijndael_cbc_t *const LS_RESTRICT ctx, const void *const LS_RESTRICT key, const size_t key_size, const void *const LS_RESTRICT iv, const uint16_t flags);
	LSAPI ls_result_t ls_rijndael_cbc_clear(ls_rijndael_cbc_t *const ctx);
	LSAPI ls_result_t ls_rijndael_cbc_reset(const ls_rijndael_cbc_t *const LS_RESTRICT ctx);

	LSAPI ls_result_t ls_rijndael_cbc_encrypt_block(const ls_rijndael_cbc_t *const LS_RESTRICT ctx, uint32_t *const LS_RESTRICT block);
	LSAPI ls_result_t ls_rijndael_cbc_decrypt_block(const ls_rijndael_cbc_t *const LS_RESTRICT ctx, uint32_t *const LS_RESTRICT block);

	


#ifdef __cplusplus
}
#endif

#endif
