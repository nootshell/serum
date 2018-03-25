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




#include "../../../core/setup.h"




#define LS_MD5_BLOCK_SIZE					64
#define LS_MD5_DIGEST_SIZE					16




typedef struct ls_md5base_data {
	uint32_t state_A;
	uint32_t state_B;
	uint32_t state_C;
	uint32_t state_D;
} ls_md5base_data_t;

typedef uint8_t ls_md5_digest_t[LS_MD5_DIGEST_SIZE];




#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t lsp_md5_init(ls_md5base_data_t *const data);

	LSAPI ls_result_t lsp_md5_update(ls_md5base_data_t *const restrict data, const uint32_t *const restrict block);

	/*!	\brief Finishes a minimal MD5 context and outputs its digest.
	**
	**	Transforms the remaining input, if any, and finishes the message
	**	with proper padding. This function also clears the context.
	**
	**	\param data The MD5 context to finish.
	**	\param input Remaining input to transform. May be `NULL` if
	**	             \p size is `0`.
	**	\param size The size of the remaining input.
	**	\param bits The number of total bits in the message for
	**	            this context.
	**	\param digest The output location for the MD5 digest.
	**
	**	\return
	**		`#LS_E_NULL` if \p data or \p digest is `NULL`.
	**		`#LS_E_NULL` if \p size is greater than `0` and \p input is null.
	**		`#LS_E_SIZE` if \p size is greater than `#LS_MD5_BLOCK_SIZE`.
	**		`#LS_E_FAILURE` if any transformations fail.
	**		`#LS_E_SUCCESS` otherwise.
	*/
	LSAPI ls_result_t lsp_md5_finish(ls_md5base_data_t *const restrict data, const uint8_t *const restrict input, size_t size, const size_t bits, ls_md5_digest_t digest);

#ifdef __cplusplus
}
#endif




#endif
