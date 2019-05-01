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
 *
 * \param[out] context The context to initialize.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_hash_init)(void *const context);

/*!
 * \brief Clears the given hash \p context.
 *
 * \param[out] context The context to clear.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_hash_clear)(void *const context);

/*!
 * \brief Updates the given hash \p context.
 *
 * \param[inout] context The context to update.
 * \param[in] block The block to consume.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_hash_update)(void *const restrict context, const uint8_t *const restrict block);

/*!
 * \brief Calculates the \p digest for the specified \p context, clearing the \p context afterwards.
 *
 * Transforms the remaining input, if any, and finishes the message with proper padding. Calculates the digest, and clears the context.
 *
 * \param[inout] context The context to use.
 * \param[in]    data Remaining input to transform. May be `NULL` if there is nothing to transform (however, \p size must be set to `0`).
 * \param[in]    size The size of the remaining input.
 * \param[out]   digest The output location of the digest.
 *
 * \return
 *		`#LS_E_NULL` if \p context or \p digest is `NULL`.					\n
 *		`#LS_E_NULL` if \p size is greater than `0` and \p input is `NULL`.	\n
 *		`#LS_E_SIZE` if \p size is greater than the algorithm's block size.	\n
 *		`#LS_E_FAILURE` if any transformations fail.						\n
 *		`#LS_E_SUCCESS` otherwise.
 */
typedef ls_result_t (*lssig_hash_finish)(void *const restrict context, const uint8_t *const restrict data, const size_t size, uint8_t *const restrict digest);




/* Cipher signatures */

/*!
 * \brief Initializes the given cipher context.
 *
 * \param[out] context The context to initialize.
 * \param[in]  key The key to use.
 * \param[in]  key_size The size of the \p key.
 * \param[in]  nonce The nonce to use.
 * \param[in]  nonce_size The size of the \p nonce.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_cipher_init)(void *const restrict context, const uint8_t *const restrict key, const size_t key_size, const uint8_t *const restrict nonce, const size_t nonce_size);

/*!
 * \brief Clears the given cipher \p context.
 *
 * \param[out] context The context to clear.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_cipher_clear)(void *const context);

/*!
 * \brief Rekeys the given cipher \p context.
 *
 * \param[inout] context The context to rekey.
 * \param[in]    key The key to use.
 * \param[in]    key_size The size of the \p key.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_cipher_rekey)(void *const restrict context, const uint8_t *const restrict key, const size_t key_size);

/*!
 * \brief Renonces the given cipher \p context.
 *
 * \param[inout] context The context to renonce.
 * \param[in]    nonce The nonce to use.
 * \param[in]    nonce_size The size of the \p nonce.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_cipher_renonce)(void *const restrict context, const uint8_t *const restrict nonce, const size_t nonce_size);

/*!
 * \brief Fetches the current streaming block of the given cipher \p context.
 *
 * \param[inout] context The context to use.
 * \param[out]   block The variable to store the block in.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_cipher_get_block)(void *const restrict context, void *const restrict block);

/*!
 * \brief Encrypts the given \p block using the given cipher \p context.
 *
 * \param[inout] context The context to use.
 * \param[inout] block The block to encrypt.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_cipher_block_encrypt)(void *const restrict context, void *const restrict block);

/*!
 * \brief Decrypts the given \p block using the given cipher \p context.
 *
 * \param[inout] context The context to use.
 * \param[inout] block The block to decrypt.
 *
 * \return ?TODO
 */
typedef ls_result_t (*lssig_cipher_block_decrypt)(void *const restrict context, void *const restrict block);




#endif
