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




#include "../setup.h"




/* Hashing signatures */

/*!
 * \brief Initializes the given hash \p context.
 * \param context The context to initialize.
 */
typedef ls_result_t (*lssig_hash_init)(void *const context);

/*!
 * \brief Clears the given hash \p context.
 * \param context The context to clear.
 */
typedef ls_result_t (*lssig_hash_clear)(void *const context);

/*!
 * \brief Updates the given hash \p context.
 * \param context The context to update.
 * \param data The data to consume.
 */
typedef ls_result_t (*lssig_hash_update)(void *const restrict context, const uint8_t *const restrict data);

/*!
 * \brief Finishes the given hash \p context.
 * \param context The context to finish.
 * \param data [optional] Remaining data to consume.
 * \param size [optional] The size of the given \p data.
 * \param digest The variable to store the calculated digest in.
 */
typedef ls_result_t (*lssig_hash_finish)(void *const restrict context, const uint8_t *const restrict data, const size_t size, uint8_t *const restrict digest);




/* Cipher signatures */

/*!
 * \brief Initializes the given cipher \p context.
 *
 * \param context The context to initialize.
 * \param key The key to use.
 * \param key_size The size of the \p key.
 * \param nonce [optional] The nonce to use.
 * \param nonce_size [optional] The size of the \p nonce.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_cipher_init)(void *const restrict context, const uint8_t *const restrict key, const size_t key_size, const uint8_t *const restrict nonce, const size_t nonce_size);

/*!
 * \brief Clears the given cipher \p context.
 *
 * \param context The context to clear.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_cipher_clear)(void *const context);

/*!
 * \brief Rekeys the given cipher \p context.
 *
 * \param context The context to rekey.
 * \param key The key to use.
 * \param key_size The size of the \p key.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_cipher_rekey)(void *const restrict context, const uint8_t *const restrict key, const size_t key_size);

/*!
 * \brief Renonces the given cipher \p context.
 *
 * \param context The context to renonce.
 * \param nonce The nonce to use.
 * \param nonce_size The size of the \p nonce.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_cipher_renonce)(void *const restrict context, const uint8_t *const restrict nonce, const size_t nonce_size);

/*!
 * \brief Fetches the current streaming block of the given cipher \p context.
 *
 * \param context The context to use.
 * \param block The variable to store the block in.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_cipher_get_stream_block)(void *const restrict context, void *const restrict block);

/*!
 * \brief Encrypts the given \p block using the given cipher \p context.
 *
 * \param context The context to use.
 * \param block The block to encrypt.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_cipher_block_encrypt)(void *const restrict context, void *const restrict block);

/*!
 * \brief Decrypts the given \p block using the given cipher \p context.
 *
 * \param context The context to use.
 * \param block The block to decrypt.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_cipher_block_decrypt)(void *const restrict context, void *const restrict block);




#endif
