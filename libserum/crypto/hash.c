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
ls_hash_init(ls_hash_t *const context, ls_hash_algo_t algorithm) {
	if (context == NULL) {
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
	context->algo_context = ptr;

	ptr = malloc(reg_entry->block_size);
	if (ptr == NULL) {
		context->algo_context = ls_memory_free(context->algo_context);
		return_e(LS_E_MEMORY);
	}
	context->buffer = ptr;


	context->algo_context_size = reg_entry->ctx_size;
	context->buffer_size = reg_entry->block_size;
	context->buffer_index = 0;

	context->f_init = reg_entry->f_init;
	context->f_clear = reg_entry->f_clear;
	context->f_update = reg_entry->f_update;
	context->f_finish = reg_entry->f_finish;


	if (context->f_init(context->algo_context) != LS_E_SUCCESS) {
		ls_hash_clear(context);
		return_e(LS_E_INITIALIZATION);
	}


	return LS_E_SUCCESS;
}


ls_result_t
ls_hash_clear(ls_hash_t *const context) {
	if (context == NULL) {
		return_e(LS_E_NULL);
	}

	if (context->algo_context != NULL) {
		if (context->f_clear == NULL || context->f_clear(context->algo_context) != LS_E_SUCCESS) {
			context->algo_context = ls_memory_clear_free(context->algo_context, context->algo_context_size);
		}
	}

	if (context->buffer != NULL) {
		context->buffer = ls_memory_clear_free(context->buffer, context->buffer_size);
	}

	ls_memory_clear(context, sizeof(*context));

	return LS_E_SUCCESS;
}


ls_result_t
ls_hash_reinit(ls_hash_t *const context) {
	if (context == NULL) {
		return_e(LS_E_NULL);
	}

	if (context->f_clear != NULL && context->f_clear(context->algo_context) != LS_E_SUCCESS) {
		return_e(LS_E_FAILURE);
	}

	return context->f_init(context->algo_context);
}




ls_result_t
ls_hash_update(ls_hash_t *const restrict context, const uint8_t *const restrict data, const size_t size) {
	if (context == NULL || data == NULL) {
		return_e(LS_E_NULL);
	}

	if (size == 0) {
		return_e(LS_E_NOOP);
	}


	const size_t buffer_index = context->buffer_index;
	const size_t buffer_size = context->buffer_size;


	/* If we can't fill the buffer, store the data in the buffer and report success */
	if ((buffer_index + size) < buffer_size) {
		memcpy(&context->buffer[buffer_index], data, size);
		context->buffer_index += size;
		return LS_E_SUCCESS;
	}


	size_t sz_remaining = size;
	const uint8_t *dptr = data;
	LS_STACK_ALLOC(uint8_t, cache, buffer_size);


	/* Prepare the cache with buffered data (if any) plus input data */
	if (buffer_index > 0) {
		memcpy(cache, context->buffer, buffer_index);

		const size_t remaining = (buffer_size - buffer_index);
		memcpy(&cache[buffer_index], dptr, remaining);
		dptr += remaining;
		sz_remaining += buffer_index;
	} else {
		memcpy(cache, dptr, buffer_size);
		dptr += buffer_size;
	}


	void *const algctx = context->algo_context;
	for (;;) {
		/* Process the block in the cache */
		if (context->f_update(algctx, cache) != LS_E_SUCCESS) {
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


	context->buffer_index = sz_remaining;
	if (sz_remaining > 0) {
		memcpy(context->buffer, dptr, sz_remaining);
	}


	LS_STACK_FREE(cache);
	return LS_E_SUCCESS;
}


ls_result_t
ls_hash_finish(ls_hash_t *const restrict context, uint8_t *const restrict out_digest) {
	if (context == NULL || out_digest == NULL) {
		return_e(LS_E_NULL);
	}

	return context->f_finish(context->algo_context, context->buffer, context->buffer_index, out_digest);
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
