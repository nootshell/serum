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

#ifndef __LS_CRYPTO_HMAC_H
#define __LS_CRYPTO_HMAC_H


#include "../../core/stdincl.h"


typedef ls_result_t(*ls_hf_init_t)(void *const hf_data);
typedef ls_result_t(*ls_hf_update_t)(void *const hf_data, const void *const data, size_t size);
typedef ls_result_t(*ls_hf_finish_t)(void *const hf_data, void *const digest);
typedef ls_result_t(*ls_hf_clear_t)(void *const hf_data);


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_hmac_universal(const void *const LS_RESTRICT data, const size_t data_size, const void *LS_RESTRICT key, size_t key_size, void *const LS_RESTRICT digest, const size_t digest_size, const size_t block_size, void *const LS_RESTRICT hf_data, ls_hf_init_t const hf_init, ls_hf_update_t const hf_update, ls_hf_finish_t const hf_finish, ls_hf_clear_t const hf_clear);

#ifdef __cplusplus
}
#endif


#endif
