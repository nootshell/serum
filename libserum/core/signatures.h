/******************************************************************************
**                                                                           **
**   The MIT License                                                         **
**                                                                           **
**   Copyright 2017-2018 icecubetray                                         **
**                                                                           **
**   Permission is hereby granted, free of charge, to any person             **
**   obtaining a copy of this software and associated documentation files    **
**   (the "Software"), to deal in the Software without restriction,          **
**   including without limitation the rights to use, copy, modify, merge,    **
**   publish, distribute, sublicense, and/or sell copies of the Software,    **
**   and to permit persons to whom the Software is furnished to do so,       **
**   subject to the following conditions:                                    **
**                                                                           **
**   The above copyright notice and this permission notice shall be          **
**   included in all copies or substantial portions of the Software.         **
**                                                                           **
**   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,         **
**   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF      **
**   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  **
**   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    **
**   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    **
**   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       **
**   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  **
**                                                                           **
******************************************************************************/


#ifndef __LS_CRYPTO_SIGNATURES_H
#define __LS_CRYPTO_SIGNATURES_H




#include "./setup.h"




// Hash function signatures.
typedef ls_result_t (*lssig_hash_init)(void *const ctx);
typedef ls_result_t (*lssig_hash_clear)(void *const ctx);
typedef ls_result_t (*lssig_hash_update)(void *const restrict ctx, const uint8_t *const restrict data);
typedef ls_result_t (*lssig_hash_finish)(void *const restrict ctx, const uint8_t *const restrict data, const size_t size, uint8_t *const restrict digest);

// Cipher signatures.
typedef ls_result_t (*lssig_cipher_init)(void *const restrict ctx, const ls_nword_t key_size, const uint8_t *const restrict key, const uint8_t *const restrict nonce);
typedef ls_result_t (*lssig_cipher_clear)(void *const ctx);
typedef ls_result_t (*lssig_cipher_rekey)(void *const restrict ctx, const ls_nword_t key_size, const uint8_t *const restrict key);
typedef ls_result_t (*lssig_cipher_renonce)(void *const restrict ctx, const uint8_t *const restrict nonce);
typedef ls_result_t (*lssig_cipher_block_encrypt)(void *const restrict ctx, void *const restrict block);
typedef ls_result_t (*lssig_cipher_block_decrypt)(void *const restrict ctx, void *const restrict block);




#endif
