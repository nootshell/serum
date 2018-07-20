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




#include "../core/setup.h"

#include "./registry.h"




typedef struct ls_hash {
	/* Allocated memory for the context */
	void *context;
	size_t context_size;

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
	 * \brief ls_hash_init
	 * \param hash
	 * \param algorithm
	 * \return
	 */
	LSAPI ls_result_t ls_hash_init(ls_hash_t *const hash, ls_hash_algo_t algorithm);

	/*!
	 * \brief ls_hash_clear
	 * \param hash
	 * \return
	 */
	LSAPI ls_result_t ls_hash_clear(ls_hash_t *const hash);

	/*!
	 * \brief ls_hash_reinit
	 * \param hash
	 * \return
	 */
	LSAPI ls_result_t ls_hash_reinit(ls_hash_t *const hash);

	/*!
	 * \brief ls_hash_update
	 * \param hash
	 * \param data
	 * \param size
	 * \return
	 */
	LSAPI ls_result_t ls_hash_update(ls_hash_t *const restrict hash, const uint8_t *const restrict data, const size_t size);

	/*!
	 * \brief ls_hash_finish
	 * \param hash
	 * \param out_digest
	 * \return
	 */
	LSAPI ls_result_t ls_hash_finish(ls_hash_t *const restrict hash, uint8_t *const restrict out_digest);

	/*!
	 * \brief ls_hash
	 * \param algorithm
	 * \param out_digest
	 * \param data
	 * \param size
	 * \return
	 */
	LSAPI ls_result_t ls_hash(ls_hash_algo_t algorithm, uint8_t *const restrict out_digest, const uint8_t *const restrict data, const size_t size);

#ifdef __cplusplus
}
#endif




#endif
