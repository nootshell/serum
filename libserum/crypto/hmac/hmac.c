/*******************************************************************************
**                                                                            **
**   The MIT License                                                          **
**                                                                            **
**   Copyright 2017 icecubetray                                               **
**                                                                            **
**   Permission is hereby granted, free of charge, to any person              **
**   obtaining a copy of this software and associated documentation files     **
**   (the "Software"), to deal in the Software without restriction,           **
**   including without limitation the rights to use, copy, modify, merge,     **
**   publish, distribute, sublicense, and/or sell copies of the Software,     **
**   and to permit persons to whom the Software is furnished to do so,        **
**   subject to the following conditions:                                     **
**                                                                            **
**   The above copyright notice and this permission notice shall be           **
**   included in all copies or substantial portions of the Software.          **
**                                                                            **
**   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          **
**   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       **
**   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   **
**   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     **
**   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     **
**   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        **
**   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   **
**                                                                            **
********************************************************************************
**
**  Notes:
**    -
**
*/

#define FILE_PATH							"crypto/hmac/hmac.c"

#include "./hmac.h"
#include "../../core/memory.h"
#include <string.h>
#include "../../debug/memdump.h"


ID("universal pluggable HMAC implementation");


void
static LS_ATTR_INLINE prep_key(uint8_t *const LS_RESTRICT out, const size_t out_size, const uint8_t pad, const void *const LS_RESTRICT key, const size_t key_size) {
	memset(out, pad, out_size);

	ls_nword_t i;
	for (i = ((ls_nword_t)key_size); i--;) {
		out[i] ^= ((uint8_t*)key)[i];
	}
}

ls_result_t
ls_hmac_universal(const void *const LS_RESTRICT data, const size_t data_size, const void *LS_RESTRICT key, size_t key_size, void *const LS_RESTRICT digest, const size_t digest_size, const size_t block_size, void *const LS_RESTRICT hf_data, ls_hash_init_func_t const hf_init, ls_hash_update_func_t const hf_update, ls_hash_finish_func_t const hf_finish, ls_hash_clear_func_t const hf_clear) {
	LS_RESULT_CHECK_NULL(data, 1);
	LS_RESULT_CHECK_SIZE(data_size, 1);
	LS_RESULT_CHECK_NULL(key, 2);
	LS_RESULT_CHECK_SIZE(key_size, 2);
	LS_RESULT_CHECK_NULL(digest, 3);
	LS_RESULT_CHECK_SIZE(digest_size, 3);
	LS_RESULT_CHECK_SIZE(block_size, 4);
	LS_RESULT_CHECK_NULL(hf_data, 4);
	LS_RESULT_CHECK_NULL(hf_init, 5);
	LS_RESULT_CHECK_NULL(hf_update, 6);
	LS_RESULT_CHECK_NULL(hf_finish, 7);
	LS_RESULT_CHECK_NULL(hf_clear, 8);


	uint8_t stackalloc(buffer_block, block_size);
	uint8_t stackalloc(buffer_digest, digest_size);

	if (key_size > block_size) {
		hf_init(hf_data);
		hf_update(hf_data, key, key_size);
		hf_finish(hf_data, buffer_digest);
		hf_clear(hf_data);

		key = buffer_digest;
		key_size = stacksizeof(buffer_digest);
	}

	prep_key(buffer_block, stacksizeof(buffer_block), 0x36, key, key_size);
	hf_init(hf_data);
	hf_update(hf_data, buffer_block, stacksizeof(buffer_block));
	hf_update(hf_data, data, data_size);
	hf_finish(hf_data, buffer_digest);
	hf_clear(hf_data);

	prep_key(buffer_block, stacksizeof(buffer_block), 0x5C, key, key_size);
	hf_init(hf_data);
	hf_update(hf_data, buffer_block, stacksizeof(buffer_block));
	hf_update(hf_data, buffer_digest, stacksizeof(buffer_digest));
	hf_finish(hf_data, digest);
	hf_clear(hf_data);

	stackfree(buffer_block);
	stackfree(buffer_digest);


	return LS_RESULT_SUCCESS;
}
