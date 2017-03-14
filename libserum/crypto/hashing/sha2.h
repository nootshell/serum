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

#ifndef __LS_CRYPTO_HASHING_SHA2_H
#define __LS_CRYPTO_HASHING_SHA2_H


#include "../../core/stdincl.h"


struct ls_sha2_32 {
	uint32_t h[8];
};

struct ls_sha2_64 {
	uint64_t h[8];
};

typedef struct ls_sha2_32 ls_sha2_224_t;
typedef struct ls_sha2_32 ls_sha2_256_t;
typedef struct ls_sha2_64 ls_sha2_384_t;
typedef struct ls_sha2_64 ls_sha2_512_t;


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_sha2_224_init(ls_sha2_224_t *ctx);
	LSAPI ls_result_t ls_sha2_224_clear(ls_sha2_224_t *ctx);
	LSAPI ls_result_t ls_sha2_224_update(ls_sha2_224_t *ctx, const uint32_t block[16]);
	LSAPI ls_result_t ls_sha2_224_finish(ls_sha2_224_t *ctx, const uint32_t *const block, size_t size, uint8_t digest[28]);

	LSAPI ls_result_t ls_sha2_256_init(ls_sha2_256_t *ctx);
	LSAPI ls_result_t ls_sha2_256_clear(ls_sha2_256_t *ctx);
	LSAPI ls_result_t ls_sha2_256_update(ls_sha2_256_t *ctx, const uint32_t block[16]);
	LSAPI ls_result_t ls_sha2_256_finish(ls_sha2_256_t *ctx, const uint32_t *const block, size_t size, uint8_t digest[32]);

	LSAPI ls_result_t ls_sha2_384_init(ls_sha2_384_t *ctx);
	LSAPI ls_result_t ls_sha2_384_clear(ls_sha2_384_t *ctx);
	LSAPI ls_result_t ls_sha2_384_update(ls_sha2_384_t *ctx, const uint64_t block[16]);
	LSAPI ls_result_t ls_sha2_384_finish(ls_sha2_384_t *ctx, const uint64_t *const block, size_t size, uint8_t digest[48]);

	LSAPI ls_result_t ls_sha2_512_init(ls_sha2_512_t *ctx);
	LSAPI ls_result_t ls_sha2_512_clear(ls_sha2_512_t *ctx);
	LSAPI ls_result_t ls_sha2_512_update(ls_sha2_512_t *ctx, const uint64_t block[16]);
	LSAPI ls_result_t ls_sha2_512_finish(ls_sha2_512_t *ctx, const uint64_t *const block, size_t size, uint8_t digest[64]);

#ifdef __cplusplus
}
#endif


#endif
