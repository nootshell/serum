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


#include "./hash_utils.h"

#include "../core/memory.h"

#include <string.h>




FILEID("Utilities for hash functions.");




ls_result_t
__ls_hash_finish_80_00_length64(void *const restrict context, lssig_hash_update f_update, const ls_message_length_t length, uint8_t *const restrict block_buffer, const size_t block_size, const void *const restrict input, const size_t input_size) {
#if (LS_SANITY)
	if (context == NULL || f_update == NULL || block_buffer == NULL || (input_size > 0 && input == NULL)) {
		return_e(LS_E_NULL);
	}

	if (block_size == 0) {
		return_e(LS_E_SIZE);
	}
#endif


	// Populate the buffer with remaining input (if any), and the starting
	// bit of the padding.
	size_t pad_size;
	if (input_size > 0) {
		memcpy(block_buffer, input, input_size);
		pad_size = input_size;
	} else {
		pad_size = 0;
	}
	block_buffer[pad_size++] = 0x80;


	// Check if the buffer can hold the input and the final padding and if
	// not so, process what we have so far (split the finalization into two
	// transformations).
	size_t diff = (block_size - pad_size);
	if (diff < sizeof(uint64_t)) {
		if (pad_size < block_size) {
			ls_memory_clear(&block_buffer[pad_size], diff);
		}

		if (f_update(context, block_buffer) != LS_E_SUCCESS) {
			return_e(LS_E_FAILURE);
		}

		pad_size = 0;
		diff = block_size;
	}


	// Clear everything between the pad bit and the length.
	ls_memory_clear(&block_buffer[pad_size], (diff - 8));


	// Append the length of the message to the end of the pad and perform the
	// final transform.
	*((uint64_t*)(block_buffer + (block_size - 8))) = LS_ENSURE_LITTLE64(length);
	if (f_update(context, block_buffer) != LS_E_SUCCESS) {
		return_e(LS_E_FAILURE);
	}


	return LS_E_SUCCESS;
}
