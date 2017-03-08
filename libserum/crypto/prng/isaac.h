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

#ifndef __LS_CRYPTO_PRNG_ISAAC_H
#define __LS_CRYPTO_PRNG_ISAAC_H


#include "../../core/stdincl.h"
#include "./device.h"


#define LS_CRYPTO_PRNG_ISAAC_SIZEL			(8U)
#define LS_CRYPTO_PRNG_ISAAC_SIZE			(1 << LS_CRYPTO_PRNG_ISAAC_SIZEL)


typedef struct ls_prng_isaac ls_prng_isaac_t;


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_prng_isaac_init_ex(ls_prng_isaac_t *const ctx, const void *const seed, const size_t size);
	LSAPI ls_result_t ls_prng_isaac_init(ls_prng_isaac_t *const ctx);
	LSAPI ls_result_t ls_prng_isaac_init_device(ls_prng_isaac_t *const ctx, const ls_prng_device_t *const device);
	LSAPI ls_result_t ls_crypto_prng_isaac_clear(ls_prng_isaac_t *const ctx);

	LSAPI ls_result_t ls_prng_isaac_update(ls_prng_isaac_t *const ctx);

	LSAPI uint32_t ls_prng_isaac(ls_prng_isaac_t *const ctx);

#if DEBUG
	LSAPI ls_result_t ls_prng_isaac_test();
#endif

#ifdef __cplusplus
}
#endif


#endif
