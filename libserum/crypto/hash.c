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


#include "./hash.h"

#include "../core/memory.h"

#include <string.h>




FILEID("Luxury hash function wrapper.");




ls_result_t
ls_hash_init(ls_hash_t *const hash, ls_hash_algo_t algorithm) {
	if (hash == NULL) {
		return_e(LS_E_NULL);
	}

	if (!LS_HASH_ALGORITHM_VALID(algorithm)) {
		return_e(LS_E_ALGORITHM);
	}


	void *ptr = NULL;
	const lsreg_hash_t *const reg_entry = &__hash_registry[algorithm];


	ptr = malloc(reg_entry->ctx_size);
	if (ptr == NULL) {
		return_e(LS_E_MEMORY);
	}
	hash->context = ptr;

	ptr = malloc(reg_entry->block_size);
	if (ptr == NULL) {
		hash->context = ls_memory_free(hash->context);
		return_e(LS_E_MEMORY);
	}
	hash->buffer = ptr;


	hash->context_size = reg_entry->ctx_size;
	hash->buffer_size = reg_entry->block_size;
	hash->buffer_index = 0;

	hash->f_init = reg_entry->f_init;
	hash->f_clear = reg_entry->f_clear;
	hash->f_update = reg_entry->f_update;
	hash->f_finish = reg_entry->f_finish;


	if (hash->f_init(hash->context) != LS_E_SUCCESS) {
		ls_hash_clear(hash);
		return_e(LS_E_INITIALIZATION);
	}


	return LS_E_SUCCESS;
}


ls_result_t
ls_hash_clear(ls_hash_t *const hash) {
	if (hash == NULL) {
		return_e(LS_E_NULL);
	}

	if (hash->context != NULL) {
		if (hash->f_clear == NULL || hash->f_clear(hash->context) != LS_E_SUCCESS) {
			hash->context = ls_memory_clear_free(hash->context, hash->context_size);
		}
	}

	if (hash->buffer != NULL) {
		hash->buffer = ls_memory_clear_free(hash->buffer, hash->buffer_size);
	}

	ls_memory_clear(hash, sizeof(*hash));

	return LS_E_SUCCESS;
}


ls_result_t
ls_hash_reinit(ls_hash_t *const hash) {
	if (hash == NULL) {
		return_e(LS_E_NULL);
	}

	if (hash->f_clear != NULL && hash->f_clear(hash->context) != LS_E_SUCCESS) {
		return_e(LS_E_FAILURE);
	}

	return hash->f_init(hash->context);
}




ls_result_t
ls_hash_update(ls_hash_t *const restrict hash, const uint8_t *const restrict data, const size_t size) {
	if (hash == NULL || data == NULL) {
		return_e(LS_E_NULL);
	}

	if (size == 0) {
		return_e(LS_E_NOOP);
	}


	const size_t buffer_index = hash->buffer_index;
	const size_t buffer_size = hash->buffer_size;


	/* If we can't fill the buffer, store the data in the buffer and report success */
	if ((buffer_index + size) < buffer_size) {
		memcpy(&hash->buffer[buffer_index], data, size);
		hash->buffer_index += size;
		return LS_E_SUCCESS;
	}


	size_t sz_remaining = size;
	const uint8_t *dptr = data;
	LS_STACK_ALLOC(uint8_t, cache, buffer_size);


	/* Prepare the cache with buffered data (if any) plus input data */
	if (buffer_index > 0) {
		memcpy(cache, hash->buffer, buffer_index);

		const size_t remaining = (buffer_size - buffer_index);
		memcpy(&cache[buffer_index], dptr, remaining);
		dptr += remaining;
		sz_remaining += buffer_index;
	} else {
		memcpy(cache, dptr, buffer_size);
		dptr += buffer_size;
	}


	void *const context = hash->context;
	for (;;) {
		/* Process the block in the cache */
		if (hash->f_update(context, cache) != LS_E_SUCCESS) {
			// TODO: HCF
			LS_STACK_FREE(cache);
			return_e(LS_E_FAILURE);
		}
		sz_remaining -= buffer_size;

		/* If we don't have a full block anymore, break */
		if (sz_remaining < buffer_size) {
			break;
		}

		/* Load next block */
		memcpy(cache, dptr, buffer_size);
		dptr += buffer_size;
	}


	hash->buffer_index = sz_remaining;
	if (sz_remaining > 0) {
		memcpy(hash->buffer, dptr, sz_remaining);
	}


	LS_STACK_FREE(cache);
	return LS_E_SUCCESS;
}


ls_result_t
ls_hash_finish(ls_hash_t *const restrict hash, uint8_t *const restrict out_digest) {
	if (hash == NULL || out_digest == NULL) {
		return_e(LS_E_NULL);
	}

	return hash->f_finish(hash->context, hash->buffer, hash->buffer_index, out_digest);
}




ls_result_t
ls_hash(ls_hash_algo_t algorithm, uint8_t *const restrict out_digest, const uint8_t *const restrict data, const size_t size) {
	ls_hash_t hctx;
	memset(&hctx, 0, sizeof(hctx));

	ls_result_t result = LS_E_SUCCESS;


	if ((result = ls_hash_init(&hctx, algorithm)) != LS_E_SUCCESS) {
		goto __cleanup;
	}

	if ((result = ls_hash_update(&hctx, data, size)) != LS_E_SUCCESS) {
		goto __cleanup;
	}

	if ((result = ls_hash_finish(&hctx, out_digest)) != LS_E_SUCCESS) {
		goto __cleanup;
	}


__cleanup:
	ls_hash_clear(&hctx);
	return result;
}
