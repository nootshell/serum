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

#ifndef __LS_CRYPTO_PRNG_DEVICE_H
#define __LS_CRYPTO_PRNG_DEVICE_H


#include "../../core/stdincl.h"
#include <stdio.h>


typedef struct ls_prng_device {
	FILE *fp;
	uint8_t *buffer;
	size_t buffer_size;
} ls_prng_device_t;

typedef enum ls_prng_device_type {
	DEV_UNSPECIFIED		= 0,
	DEV_HARDWARE		= 1,	// Use /dev/hwrng, if available
	DEV_URANDOM			= 2,	// Allow falling back to /dev/urandom, if available
	DEV_FORCE_UNLIMITED	= 4		// Force using /dev/urandom, if available
} ls_prng_device_type_t;


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_prng_device_init(ls_prng_device_t *const device, const char *const file, size_t const buffer_size);
	LSAPI ls_result_t ls_prng_device_clear(ls_prng_device_t *const device);
	LSAPI ls_result_t ls_prng_device_generate(const ls_prng_device_t *const device, void *const out, size_t const size);
	LSAPI ls_result_t ls_prng_device_sys(ls_prng_device_t *const device, size_t const buffer_size, ls_prng_device_type_t const type);

#ifdef __cplusplus
}
#endif


#endif // __LS_CRYPTO_PRNG_DEVICE_H
