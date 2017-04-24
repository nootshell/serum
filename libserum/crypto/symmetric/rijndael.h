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

#ifndef __LS_CRYPTO_SYMMETRIC_RIJNDAEL_H
#define __LS_CRYPTO_SYMMETRIC_RIJNDAEL_H


#include "../../core/stdincl.h"
#include "../storage/key.h"


typedef struct ls_rijndael {
	ls_key_t *key_encrypt;
	ls_key_t *key_decrypt;
	uint16_t flags;
	uint16_t rounds;
} ls_rijndael_t;

#define LS_RIJNDAEL_BLOCK_SIZE				16

#define LS_RIJNDAEL_128						BIT_1	// Read-only
#define LS_RIJNDAEL_192						BIT_2	// Read-only
#define LS_RIJNDAEL_256						BIT_3	// Read-only


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_rijndael_init(ls_rijndael_t *const LS_RESTRICT ctx, const void *const LS_RESTRICT key, const size_t key_size);
	LSAPI ls_result_t ls_rijndael_clear(ls_rijndael_t *const ctx);
	LSAPI ls_result_t ls_rijndael_encrypt_block(const ls_rijndael_t *const LS_RESTRICT ctx, uint32_t *const LS_RESTRICT block);
	LSAPI ls_result_t ls_rijndael_decrypt_block(const ls_rijndael_t *const LS_RESTRICT ctx, uint32_t *const LS_RESTRICT block);

#ifdef __cplusplus
}
#endif


#endif
