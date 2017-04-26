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

#ifndef __LS_CRYPTO_KEY_EXCHANGE_X25519_H
#define __LS_CRYPTO_KEY_EXCHANGE_X25519_H


#include "../../core/stdincl.h"
#include "../storage/key.h"


#define LS_X25519_KEY_SIZE					32


typedef uint8_t ls_x25519_key_t[LS_X25519_KEY_SIZE];

typedef struct ls_x25519 {
	ls_key_t *private_key;
	ls_key_t *shared_key;
	ls_x25519_key_t public_key;
	ls_x25519_key_t basepoint;
} ls_x25519_t;


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_x25519_init_ex(ls_x25519_t *const ctx, const ls_x25519_key_t private_key, const ls_x25519_key_t basepoint);
	LSAPI ls_result_t ls_x25519_init(ls_x25519_t *const ctx, const ls_x25519_key_t private_key);
	LSAPI ls_result_t ls_x25519_clear(ls_x25519_t *const ctx);

	LSAPI ls_result_t ls_x25519_generate_shared(const ls_x25519_t *const ctx, const ls_x25519_key_t public_key);

	LSAPI const void* ls_x25519_get_public(const ls_x25519_t *const ctx);
	LSAPI const void* ls_x25519_get_private(const ls_x25519_t *const ctx);
	LSAPI const void* ls_x25519_get_shared(const ls_x25519_t *const ctx);

#ifdef __cplusplus
}
#endif


#endif
