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

#ifndef __LS_CRYPTO_HASHING_MD5_H
#define __LS_CRYPTO_HASHING_MD5_H


#include "../../core/stdincl.h"


#define LS_MD5_BLOCK_SIZE					64
#define LS_MD5_DIGEST_SIZE					16


typedef struct ls_md5 {
	uint32_t h[4];
	uint32_t hi;
	uint32_t lo;
	uint32_t __psize;
	uint8_t __pcache[LS_MD5_BLOCK_SIZE];
} ls_md5_t;

typedef uint8_t ls_md5_digest_t[LS_MD5_DIGEST_SIZE];


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_md5_init(ls_md5_t *const ctx);
	LSAPI ls_result_t ls_md5_clear(ls_md5_t *const ctx);
	LSAPI ls_result_t ls_md5_update(ls_md5_t *const LS_RESTRICT ctx, const void *const LS_RESTRICT data, size_t size);
	LSAPI ls_result_t ls_md5_finish(ls_md5_t *const ctx, ls_md5_digest_t digest);

#ifdef __cplusplus
}
#endif


#endif
