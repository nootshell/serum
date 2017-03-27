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

#define FILE_PATH							"crypto/storage/key.c"

#include "./key.h"
#include "../../core/memory.h"
#include <string.h>


ID("key storage");


void
static LS_ATTR_INLINE __ls_key_clear(ls_key_t *const key) {
	memset(key->data, 0, key->size);
	key->size = 0;
}


ls_result_t
ls_key_init(ls_key_t *const key, const size_t size) {
	LS_RESULT_CHECK_NULL(key, 1);
	LS_RESULT_CHECK_SIZE(size, 1);

	__ls_key_clear(key);

	key->size = size;

	if (!LS_MEMLOCK(key, (sizeof(*key) + key->size))) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_LOCK);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_key_clear(ls_key_t *const key) {
	LS_RESULT_CHECK_NULL(key, 1);
	LS_RESULT_CHECK_SIZE(key->size, 1);

	size_t size = key->size;

	__ls_key_clear(key);

	if (!LS_MEMUNLOCK(key, (sizeof(*key) + size))) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_LOCK);
	}

	return LS_RESULT_SUCCESS;
}


ls_key_t*
ls_key_alloc(const size_t size) {
	ls_key_t *key = malloc(sizeof(*key) + size);

	if (ls_key_init(key, size).success) {
		return key;
	} else {
		if (key) {
			free(key);
		}
		return NULL;
	}
}


ls_key_t*
ls_key_alloc_from(const void *const in, const size_t size) {
	if (!in || !size) {
		return NULL;
	}

	ls_key_t *key = ls_key_alloc(size);
	if (key) {
		if (!ls_key_set(key, in, 0, size).success) {
			return ls_key_free(key);
		}
	}
	return key;
}


ls_key_t*
ls_key_clone(const ls_key_t *const src) {
	if (!src) {
		return NULL;
	}

	if (!src->size) {
		return NULL;
	}

	return ls_key_alloc_from(src->data, src->size);
}


ls_key_t*
ls_key_free(ls_key_t *const key) {
	if (key) {
		__ls_key_clear(key);
		free(key);
	}
	return NULL;
}


ls_result_t
ls_key_set(const ls_key_t *const LS_RESTRICT key, const void *const LS_RESTRICT in, const uintptr_t offset, const size_t size) {
	LS_RESULT_CHECK_NULL(key, 1);
	LS_RESULT_CHECK_NULL(in, 2);
	LS_RESULT_CHECK_SIZE(size, 1);
	LS_RESULT_CHECK_SIZE(key->size, 2);
	LS_RESULT_CHECK((size > key->size), LS_RESULT_CODE_SIZE, 3);
	LS_RESULT_CHECK(((offset + size) > key->size), LS_RESULT_CODE_INDEX, 1);

	memcpy((((char*)key->data) + offset), in, size);

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_key_get(const ls_key_t *const LS_RESTRICT key, void *const LS_RESTRICT out, const uintptr_t offset, const size_t size) {
	LS_RESULT_CHECK_NULL(key, 1);
	LS_RESULT_CHECK_NULL(out, 2);
	LS_RESULT_CHECK_SIZE(key->size, 1);
	LS_RESULT_CHECK((size > key->size), LS_RESULT_CODE_SIZE, 2);
	LS_RESULT_CHECK(((offset + size) > key->size), LS_RESULT_CODE_INDEX, 1);

	memcpy(out, (((char*)key->data) + offset), size);

	return LS_RESULT_SUCCESS;
}
