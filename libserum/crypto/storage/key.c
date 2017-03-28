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


ls_result_t
ls_key_init(ls_key_t *const key, const size_t size) {
	LS_RESULT_CHECK_NULL(key, 1);
	LS_RESULT_CHECK_SIZE(size, 1);

	key->size = size;

	memset(key->data, 0, key->size);

	if (!LS_MEMLOCK(key, (sizeof(*key) + key->size))) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_LOCK);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_key_clear(ls_key_t *const key) {
	LS_RESULT_CHECK_NULL(key, 1);
	LS_RESULT_CHECK_SIZE(key->size, 1);

	// First we clear...
	memset(key->data, 0, key->size);

	// ... then we unlock.
	if (!LS_MEMUNLOCK(key, (sizeof(*key) + key->size))) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_LOCK);
	}

	return LS_RESULT_SUCCESS;
}


ls_key_t*
ls_key_alloc(const size_t size) {
	if (!size) {
		return NULL;
	}

	ls_key_t *key = malloc(sizeof(*key) + size);
	if (ls_key_init(key, size).success) {
		return key;
	} else {
		if (key) {
			free(key);
		}
	}

	return NULL;
}


ls_key_t*
ls_key_alloc_from(const void *const in, const size_t size) {
	if (!in || !size) {
		return NULL;
	}

	ls_key_t *key = ls_key_alloc(size);
	if (key) {
		memcpy(key->data, in, size);
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
		memset(key->data, 0, key->size);
		free(key);
	}
	return NULL;
}
