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

#ifndef __LS_CRYPTO_XXTEA_H
#define __LS_CRYPTO_XXTEA_H


#include "../../core/stdincl.h"
#include "../enum.h"
#include "../storage/key.h"


typedef struct ls_xxtea {
	ls_key_t *key;
} ls_xxtea_t;


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_xxtea_init(ls_xxtea_t *const LS_RESTRICT ctx, const void *const LS_RESTRICT key);
	LSAPI ls_result_t ls_xxtea_clear(ls_xxtea_t *const ctx);
	LSAPI ls_result_t ls_xxtea_encrypt_block(const ls_xxtea_t *const LS_RESTRICT ctx, uint32_t *const LS_RESTRICT block, const size_t block_count);
	LSAPI ls_result_t ls_xxtea_decrypt_block(const ls_xxtea_t *const LS_RESTRICT ctx, uint32_t *const LS_RESTRICT block, const size_t block_count);

#ifdef __cplusplus
}
#endif


#endif
