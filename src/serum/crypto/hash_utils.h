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


#ifndef __LS_CRYPTO_HASH_UTILS_H
#define __LS_CRYPTO_HASH_UTILS_H




#include "../core/setup.h"




#ifdef __cplusplus
extern "C" {
#endif

	/*!
	 * \brief Finishes a hash context with `0x80,{0x00},<u64mesglen>`.
	 *
	 * Pads the remaining data with one bit (`0x80`), filling the remainder of the buffer to the last 8 bytes with zeroes, and adding the total length of the message in those 8 bytes as a 64-bit little-endian integer.
	 *
	 * \param[inout] context The hash context to finish.
	 * \param[in]    f_update The hash update function to use.
	 * \param[in]    length The length of the message.
	 * \param[inout] block_buffer The block to use as a buffer.
	 * \param[in]    block_size The size of the block.
	 * \param[in]    input The remaining input to process.
	 * \param[in]    input_size The size of the remaining input.
	 *
	 * \return
	 *		`#LS_E_NULL` if \p context, \p f_update, or \p block_buffer, or, if \p input_size is greater than `0`, \p input is `NULL`.	\n
	 *		`#LS_E_SIZE` if \p block_size is `0`.																						\n
	 *		`#LS_E_FAILURE` if \p f_update doesn't return `#LS_E_SUCCESS`.																\n
	 *		`#LS_E_SUCCESS` otherwise.`
	 *
	 * \visibility{internal}
	 */
	LSINT ls_result_t __ls_hash_finish_80_00_length64(void *const restrict context, lssig_hash_update f_update, const ls_message_length_t length, uint8_t *const restrict block_buffer, const size_t block_size, const void *const restrict input, const size_t input_size);

#ifdef __cplusplus
}
#endif




#endif
