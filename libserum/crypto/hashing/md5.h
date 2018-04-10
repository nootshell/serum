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


#ifndef __LS_CRYPTO_HASHING_MD5_H
#define __LS_CRYPTO_HASHING_MD5_H




#include "../../core/setup.h"




#define LS_MD5_BLOCK_SIZE					64
#define LS_MD5_DIGEST_SIZE					16




typedef struct ls_md5_data {
	size_t length;
	uint32_t state_A;
	uint32_t state_B;
	uint32_t state_C;
	uint32_t state_D;
} ls_md5_data_t;

/*! \brief A byte array with the size of a MD5 digest (`#LS_MD5_DIGEST_SIZE`). */
typedef uint8_t ls_md5_digest_t[LS_MD5_DIGEST_SIZE];
typedef uint8_t ls_md5_block_t[LS_MD5_BLOCK_SIZE];




#ifdef __cplusplus
extern "C" {
#endif

	/*! \brief Initializes the specified MD5 \p context.
	**
	**	\param context The MD5 context.
	**
	**	\return
	**		`#LS_E_NULL` if \p context is `NULL`.
	**		`#LS_E_SUCCESS` otherwise.
	*/
	LSAPI ls_result_t ls_md5_init(ls_md5_data_t *const context);

	/*!	\brief Performs a MD5 transformation using the specified \p block, for the specified \p context.
	**
	**	\param context The MD5 context.
	**	\param block The block to use.
	**
	**	\return
	**		`#LS_E_NULL` if \p context or \p block is `NULL`.
	**		`#LS_E_SUCCESS` otherwise.
	*/
	LSAPI ls_result_t ls_md5_update(ls_md5_data_t *const restrict context, const ls_md5_block_t block);

	/*!	\brief Finalizes the specified MD5 \p context by calculating its
	**	       \p digest and clearing the \p context.
	**
	**	Transforms the remaining input, if any, and finishes the message
	**	with proper padding. Calculates the digest, and clears the context.
	**
	**	\param context The MD5 context.
	**	\param input Remaining input to transform. May be `NULL` if
	**	             there is nothing to transform (\p size must be `0`).
	**	\param size The size of the remaining input.
	**	\param bits The number of total bits in the message for
	**	            this context.
	**	\param digest The output location for the MD5 digest.
	**
	**	\return
	**		`#LS_E_NULL` if \p context or \p digest is `NULL`.
	**		`#LS_E_NULL` if \p size is greater than `0` and \p input is `NULL`.
	**		`#LS_E_SIZE` if \p size is greater than `#LS_MD5_BLOCK_SIZE`.
	**		`#LS_E_FAILURE` if any transformations fail.
	**		`#LS_E_SUCCESS` otherwise.
	*/
	LSAPI ls_result_t ls_md5_finish(ls_md5_data_t *const restrict context, const uint8_t *const restrict input, size_t size, ls_md5_digest_t digest);

#ifdef __cplusplus
}
#endif




#endif
