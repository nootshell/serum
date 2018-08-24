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


#ifndef __LS_CRYPTO_HASH_H
#define __LS_CRYPTO_HASH_H




/*!
 * \page hash_wrapper Hash wrapper
 *
 * \see crypto/hash.h
 *
 * ?TODO
 */




#include "../core/setup.h"

#include "./registry.h"




typedef struct ls_hash {
	/* Allocated memory for the context */
	void *algo_context;
	size_t algo_context_size;

	/* Allocated memory for a buffer block */
	uint8_t *buffer;
	size_t buffer_size;
	size_t buffer_index;

	/* Functions */
	lssig_hash_init f_init;
	lssig_hash_clear f_clear;
	lssig_hash_update f_update;
	lssig_hash_finish f_finish;
} ls_hash_t;




#ifdef __cplusplus
extern "C" {
#endif

	/*!
	 * \brief Initialize the specified hash \p context for the specified \p algorithm.
	 *
	 * \param[inout] context The context to initialize.
	 * \param[in]    algorithm The algorithm to prepare the \p context for.
	 *
	 * \see lssig_hash_init
	 *
	 * \return
	 *		`#LS_E_NULL` if \p context is `NULL`.														\n
	 *		`#LS_E_ALGORITHM` if \p algorithm is not a valid, registered hash algorithm.				\n
	 *		`#LS_E_MEMORY` if allocating memory failed.													\n
	 *		`#LS_E_INITIALIZATION` if the initialization function of the specified \p algorithm fails.	\n
	 *		`#LS_E_SUCCESS` otherwise.
	 */
	LSAPI ls_result_t ls_hash_init(ls_hash_t *const context, ls_hash_algo_t algorithm);

	/*!
	 * \brief Clears the specified hash \p context.
	 *
	 * \param[inout] context The context to clear.
	 *
	 * \see lssig_hash_clear
	 *
	 * \return
	 *		`#LS_E_NULL` if \p context is `NULL`.	\n
	 *		`#LS_E_SUCCESS` otherwise.
	 */
	LSAPI ls_result_t ls_hash_clear(ls_hash_t *const context);

	/*!
	 * \brief Reinitializes the specified hash \p context.
	 *
	 * \param[inout] context The context to reinitialize.
	 *
	 * \see lssig_hash_init
	 *
	 * \return
	 *		`#LS_E_NULL` if \p context is `NULL`.							\n
	 *		`#LS_E_FAILURE` if clearing \p context, when possible, fails.	\n
	 *		Otherwise, the return value of the algorithm's initialization function is returned.
	 */
	LSAPI ls_result_t ls_hash_reinit(ls_hash_t *const context);

	/*!
	 * \brief Updates the given hash \p context.
	 *
	 * \param[inout] context The context to update.
	 * \param[in]    data The data to feed the \p context.
	 * \param[in]    size The number of bytes to read from \p data.
	 *
	 * \see lssig_hash_update
	 *
	 * \return
	 *		`#LS_E_NULL` if \p context or \p data is `NULL`.			\n
	 *		`#LS_E_NOOP` if \p size is `0`.								\n
	 *		`#LS_E_FAILURE` if the algorithm's update function fails.	\n
	 *		`#LS_E_SUCCESS` otherwise.
	 */
	LSAPI ls_result_t ls_hash_update(ls_hash_t *const restrict context, const uint8_t *const restrict data, const size_t size);

	/*!
	 * \brief Finishes the given hash \p context.
	 *
	 * \param[inout] context The context to finish.
	 * \param[out]   out_digest The output location of the digest.
	 *
	 * \see lssig_hash_finish
	 *
	 * \return
	 *		`#LS_E_NULL` if \p context or \p out_digest is `NULL`.							\n
	 *		Otherwise, the return value of the algorithm's finishing function is returned.
	 */
	LSAPI ls_result_t ls_hash_finish(ls_hash_t *const restrict context, uint8_t *const restrict out_digest);

	/*!
	 * \brief Calculates \p out_digest by processing \p size bytes from \p data using the specified \p algorithm.
	 *
	 * \param[in]  algorithm The algorithm to use.
	 * \param[out] out_digest The output location of the digest.
	 * \param[in]  data The data to process.
	 * \param[in]  size The number of bytes to read from \p data.
	 *
	 * \see ls_hash_init
	 * \see ls_hash_update
	 * \see ls_hash_finish
	 *
	 * \return
	 *		Any of the return values from `#ls_hash_init`, `#ls_hash_update`, and `#ls_hash_finish`.
	 */
	LSAPI ls_result_t ls_hash(ls_hash_algo_t algorithm, uint8_t *const restrict out_digest, const uint8_t *const restrict data, const size_t size);

#ifdef __cplusplus
}
#endif




#endif
