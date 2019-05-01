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




/*!
 * \page md5 MD5
 *
 * \see crypto/hashing/md5.h
 *
 * ?TODO
 */




#include "../../core/setup.h"




#define LS_MD5_BLOCK_SIZE					64
#define LS_MD5_DIGEST_SIZE					16




typedef struct ls_md5 {
	ls_message_length_t length;
	uint32_t state_A;
	uint32_t state_B;
	uint32_t state_C;
	uint32_t state_D;
} ls_md5_t;

//! \brief A byte array with the size of a MD5 block (`#LS_MD5_BLOCK_SIZE`).
typedef uint8_t ls_md5_block_t[LS_MD5_BLOCK_SIZE];

//! \brief A byte array with the size of a MD5 digest (`#LS_MD5_DIGEST_SIZE`).
typedef uint8_t ls_md5_digest_t[LS_MD5_DIGEST_SIZE];




#ifdef __cplusplus
extern "C" {
#endif

	/*!
	 * \copydoc lssig_hash_init
	 *
	 * \algorithm{MD5}
	 */
	LSAPI ls_result_t ls_md5_init(ls_md5_t *const context);

	/*!
	 * \copydoc lssig_hash_update
	 *
	 * \algorithm{MD5}
	 */
	LSAPI ls_result_t ls_md5_update(ls_md5_t *const restrict context, const ls_md5_block_t block);

	/*!
	 * \copydoc lssig_hash_finish
	 *
	 * \algorithm{MD5}
	 */
	LSAPI ls_result_t ls_md5_finish(ls_md5_t *const restrict context, const uint8_t *const restrict data, size_t size, ls_md5_digest_t digest);

#ifdef __cplusplus
}
#endif




#endif
