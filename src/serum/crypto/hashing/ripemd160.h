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


#ifndef __LS_CRYPTO_HASHING_RIPEMD160_H
#define __LS_CRYPTO_HASHING_RIPEMD160_H



/*!
 * \page ripemd160 RIPEMD160
 *
 * \see crypto/hashing/ripemd160.h
 *
 * ?TODO
 */




#include "../../core/setup.h"




#define LS_RIPEMD160_BLOCK_SIZE				64
#define LS_RIPEMD160_DIGEST_SIZE			20




typedef struct ls_ripemd160 {
	ls_message_length_t length;
	uint32_t state_A;
	uint32_t state_B;
	uint32_t state_C;
	uint32_t state_D;
	uint32_t state_E;
	uint32_t __pad;
} ls_ripemd160_t;

//! \brief A byte array with the size of a RIPEMD-160 block (`#LS_RIPEMD160_BLOCK_SIZE`).
typedef uint8_t ls_ripemd160_block_t[LS_RIPEMD160_BLOCK_SIZE];

//! \brief A byte array with the size of a RIPEMD-160 digest (`#LS_RIPEMD160_DIGEST_SIZE`).
typedef uint8_t ls_ripemd160_digest_t[LS_RIPEMD160_DIGEST_SIZE];




#ifdef __cplusplus
extern "C" {
#endif

	/*!
	 * \copydoc lssig_hash_init
	 *
	 * \algorithm{RIPEMD160}
	 */
	LSAPI ls_result_t ls_ripemd160_init(ls_ripemd160_t *const context);

	/*!
	 * \copydoc lssig_hash_update
	 *
	 * \algorithm{RIPEMD160}
	 */
	LSAPI ls_result_t ls_ripemd160_update(ls_ripemd160_t *const context, const ls_ripemd160_block_t block);

	/*!
	 * \copydoc lssig_hash_finish
	 *
	 * \algorithm{RIPEMD160}
	 */
	LSAPI ls_result_t ls_ripemd160_finish(ls_ripemd160_t *const restrict context, const uint8_t *const restrict data, const size_t size, ls_ripemd160_digest_t digest);

#ifdef __cplusplus
}
#endif




#endif
