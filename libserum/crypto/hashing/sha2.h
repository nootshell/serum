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


#define LS_SHA2_BLOCK_NUM					16
#define LS_SHA2_224_BLOCK_SIZE				LS_SHA2_256_BLOCK_SIZE
#define LS_SHA2_256_BLOCK_SIZE				(sizeof(uint32_t) * LS_SHA2_BLOCK_NUM)
#define LS_SHA2_384_BLOCK_SIZE				LS_SHA2_512_BLOCK_SIZE
#define LS_SHA2_512_BLOCK_SIZE				(sizeof(uint64_t) * LS_SHA2_BLOCK_NUM)

#define LS_SHA2_224_DIGEST_SIZE				28
#define LS_SHA2_256_DIGEST_SIZE				32
#define LS_SHA2_384_DIGEST_SIZE				48
#define LS_SHA2_512_DIGEST_SIZE				64


typedef struct ls_sha2_32 {
	uint64_t size;
	uint32_t h[8];
	uint32_t __psize;
	uint8_t __pcache[(LS_SHA2_BLOCK_NUM * sizeof(uint32_t)) - 1];
	uint8_t pad; // No real use for this byte: it would be used as a structure
	             // padding. I figured I could make this useful to some tiny
	             // extent by using this value as the pad byte in the *_finish
	             // routines.
} ls_sha2_224_t, ls_sha2_256_t;

typedef struct ls_sha2_64 {
	uint64_t size;
	uint64_t h[8];
	uint32_t __psize;
	uint8_t __pcache[(LS_SHA2_BLOCK_NUM * sizeof(uint64_t)) - 1];
	uint8_t pad; // See: (struct ls_sha2_32).pad
} ls_sha2_384_t, ls_sha2_512_t;

typedef uint8_t ls_sha2_224_digest[LS_SHA2_224_DIGEST_SIZE];
typedef uint8_t ls_sha2_256_digest[LS_SHA2_256_DIGEST_SIZE];
typedef uint8_t ls_sha2_384_digest[LS_SHA2_384_DIGEST_SIZE];
typedef uint8_t ls_sha2_512_digest[LS_SHA2_512_DIGEST_SIZE];


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_sha2_224_init(ls_sha2_224_t *const ctx);
	LSAPI ls_result_t ls_sha2_224_clear(ls_sha2_224_t *const ctx);
	LSAPI ls_result_t ls_sha2_224_update(ls_sha2_224_t *const restrict ctx, const void *const restrict in, size_t size);
	LSAPI ls_result_t ls_sha2_224_finish(ls_sha2_224_t *const ctx, ls_sha2_224_digest digest);

	LSAPI ls_result_t ls_sha2_256_init(ls_sha2_256_t *const ctx);
	LSAPI ls_result_t ls_sha2_256_clear(ls_sha2_256_t *const ctx);
	LSAPI ls_result_t ls_sha2_256_update(ls_sha2_256_t *const restrict ctx, const void *const restrict in, size_t size);
	LSAPI ls_result_t ls_sha2_256_finish(ls_sha2_256_t *const ctx, ls_sha2_256_digest digest);

	LSAPI ls_result_t ls_sha2_384_init(ls_sha2_384_t *const ctx);
	LSAPI ls_result_t ls_sha2_384_clear(ls_sha2_384_t *const ctx);
	LSAPI ls_result_t ls_sha2_384_update(ls_sha2_384_t *const restrict ctx, const void *const restrict in, size_t size);
	LSAPI ls_result_t ls_sha2_384_finish(ls_sha2_384_t *const ctx, ls_sha2_384_digest digest);

	LSAPI ls_result_t ls_sha2_512_init(ls_sha2_512_t *const ctx);
	LSAPI ls_result_t ls_sha2_512_clear(ls_sha2_512_t *const ctx);
	LSAPI ls_result_t ls_sha2_512_update(ls_sha2_512_t *const restrict ctx, const void *const restrict in, size_t size);
	LSAPI ls_result_t ls_sha2_512_finish(ls_sha2_512_t *const ctx, ls_sha2_512_digest digest);

#ifdef __cplusplus
}
#endif


#endif
