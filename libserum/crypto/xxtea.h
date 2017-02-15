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


#include "../core/stdincl.h"
#include "./enum.h"


#ifdef __cplusplus
extern "C" {
#endif

	ls_result_t ls_xxtea_encrypt_ex(void *const out, const void *const in, const size_t size, const void *const key, const crypto_pad_type_t padding);
	ls_result_t ls_xxtea_encrypt(void *const out, const void *const in, const size_t size, const void *const key);
	ls_result_t ls_xxtea_decrypt_ex(void *const out, const void *const in, const size_t size, const void *const key, const crypto_pad_type_t padding);
	ls_result_t ls_xxtea_decrypt(void *out, const void *const in, const size_t size, const void *const key);

#ifdef __cplusplus
}
#endif


#endif
