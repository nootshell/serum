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

#ifndef __LS_CRYPTO_STORAGE_KEY_H
#define __LS_CRYPTO_STORAGE_KEY_H


#include "../../core/stdincl.h"
#include "../../core/detect_compiler.h"


typedef struct ls_key {
	size_t size;
	uint8_t data[];
} ls_key_t;


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_key_init(ls_key_t *const key, const size_t size);
	LSAPI ls_result_t ls_key_clear(ls_key_t *const key);
	LSAPI ls_key_t* ls_key_alloc(const size_t size);
	LSAPI ls_key_t* ls_key_alloc_from(const void *const in, const size_t size);
	LSAPI ls_key_t* ls_key_clone(const ls_key_t *const src);
	LSAPI ls_key_t* ls_key_free(ls_key_t *const key);
	LSAPI ls_result_t ls_key_set(const ls_key_t *const LS_RESTRICT key, const void *const LS_RESTRICT in, const uintptr_t offset, const size_t size);
	LSAPI ls_result_t ls_key_get(const ls_key_t *const LS_RESTRICT key, void *const LS_RESTRICT out, const uintptr_t offset, const size_t size);

#ifdef __cplusplus
}
#endif


#endif
