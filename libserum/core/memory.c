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

#define FILE_PATH							"core/memory.c"

#include "./memory.h"
#include "./stdincl.h"
#include "./logging/log.h"
#include "../debug/memdump.h"
#include <string.h>


ID("memory related routines, semi-managed memory");


void
ls_memory_destroy(void *const ptr, size_t size) {
	if (!ptr || !size) {
		return;
	}

#if (LS_MEMORY_DESTROY_ZEROES)
#	define value							0
#else
	const unsigned char value = ((((uintptr_t)ptr) >> (((uintptr_t)ptr) & 0xFF)) & 0xFF);
#endif
	do {
		--size;
		((volatile unsigned char *const)ptr)[size] = value;
	} while (size);
}


ls_bool
ls_memequ(const void *const LS_RESTRICT cmp1, const void *const LS_RESTRICT cmp2, size_t size) {
	if (cmp1 == cmp2) {
		return true;
	}

	const uint64_t
		*LS_RESTRICT cn1 = cmp1,
		*LS_RESTRICT cn2 = cmp2;

	if (size >= sizeof(*cn1)) {
		do {
			if (*(cn1++) != *(cn2++)) {
				return false;
			}
		} while ((size -= sizeof(*cn1)) > sizeof(*cn1));
	}

	if (size) {
		const char
			*const c1 = (char*)cn1,
			*const c2 = (char*)cn2;

		for (; size--;) {
			if (c1[size] != c2[size]) {
				return false;
			}
		}
	}

	return true;
}


ls_result_t
ls_memory_area_init_ex(ls_memory_area_t *const area, const size_t size, const ls_nword_t flags, const ls_nword_t guard) {
	LS_RESULT_CHECK_NULL(area, 1);
	LS_RESULT_CHECK_SIZE(size, 1);

	if (!ls_mutex_init(&area->perm_mutex).success) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_MUTEX);
	}

	area->flags = 0;
	area->size = size;
	area->__guard = guard;
	area->__location = malloc(area->size + (sizeof(area->__guard) << 1));

	if (!area->__location) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_ALLOCATION);
	}

	area->data = (void*)(((uintptr_t)area->__location) + sizeof(area->__guard));

	*((ls_nword_t*)area->__location) = LS_SWAP_ENDIAN_BIG_32(area->__guard);
	*((ls_nword_t*)(((uintptr_t)area->data) + area->size)) = LS_SWAP_ENDIAN_BIG_32(area->__guard);

	return ls_memory_area_set_flags(area, flags);
}


ls_result_t
ls_memory_area_init(ls_memory_area_t *const area, const size_t size) {
	return ls_memory_area_init_ex(area, size, LS_MEMORY_DEFAULT_FLAGS, LS_MEMORY_GUARD_WORD);
}


ls_result_t
ls_memory_area_wrap_ex(ls_memory_area_t *const LS_RESTRICT area, void *const LS_RESTRICT ptr, const size_t size, const ls_nword_t flags) {
	LS_RESULT_CHECK_NULL(area, 1);
	LS_RESULT_CHECK_NULL(ptr, 2);
	LS_RESULT_CHECK_SIZE(size, 1);

	if (!ls_mutex_init(&area->perm_mutex).success) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_MUTEX);
	}

	area->flags = LS_MEMORY_WRAPPED;
	area->size = size;
	area->__guard = 0;
	area->__location = area->data = ptr;

	return ls_memory_area_set_flags(area, flags);
}


ls_result_t
ls_memory_area_wrap(ls_memory_area_t *const LS_RESTRICT area, void *const LS_RESTRICT ptr, const size_t size) {
	return ls_memory_area_wrap_ex(area, ptr, size, LS_MEMORY_DEFAULT_FLAGS);
}


ls_result_t
ls_memory_area_clear(ls_memory_area_t *const area) {
	LS_RESULT_CHECK_NULL(area, 1);
	if (area->__location) {
		if (!ls_mutex_lock(&area->perm_mutex).success) {
			return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_MUTEX, 1);
		}

		const size_t actual_size = (area->size + (HAS_FLAG(area->flags, LS_MEMORY_WRAPPED) ? 0 : (sizeof(area->__guard) << 1)));

		if (HAS_FLAG(area->flags, LS_MEMORY_DESTROY)) {
			ls_memory_destroy(area->__location, actual_size);
		}

		if (HAS_FLAG(area->flags, LS_MEMORY_LOCKED)) {
			if (!LS_MEMUNLOCK(area->__location, actual_size)) {
				ls_mutex_unlock(&area->perm_mutex);
				return LS_RESULT_ERROR(LS_RESULT_CODE_LOCK);
			}
		}

		if (!HAS_FLAG(area->flags, LS_MEMORY_WRAPPED)) {
			free(area->__location);
		}

		area->__location = NULL;

		if (!ls_mutex_unlock(&area->perm_mutex).success) {
			return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_MUTEX, 2);
		}
	}

	// TODO: clear struct

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_memory_area_set_flags(ls_memory_area_t *const area, const ls_nword_t flags) {
	LS_RESULT_CHECK_NULL(area, 1);

	if (!ls_mutex_lock(&area->perm_mutex).success) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_MUTEX, 1);
	}

	const size_t actual_size = (area->size + (sizeof(area->__guard) << 1));
	ls_result_t result;

	if (HAS_FLAG(area->flags, LS_MEMORY_LOCKED) != HAS_FLAG(flags, LS_MEMORY_LOCKED)) {
		if (HAS_FLAG(flags, LS_MEMORY_LOCKED)) {
			if (!LS_MEMLOCK(area->__location, actual_size)) {
				result = LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_LOCK, 1);
				goto __cleanup;
			}

			area->flags |= LS_MEMORY_LOCKED;
		} else {
			if (!LS_MEMUNLOCK(area->__location, actual_size)) {
				result = LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_LOCK, 2);
				goto __cleanup;
			}

			area->flags &= ~LS_MEMORY_LOCKED;
		}
	}

	// These flags require no special operations and will simply be set.
	area->flags |= (flags & (LS_MEMORY_DESTROY));

	result = LS_RESULT_SUCCESS;

__cleanup:
	if (!ls_mutex_unlock(&area->perm_mutex).success) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_MUTEX, 2);
	}

	return result;
}


ls_result_t
ls_memory_area_set_swappable(ls_memory_area_t *const area, const ls_bool swappable) {
	return ls_memory_area_set_flags(area, (swappable ? (area->flags & ~LS_MEMORY_UNSWAPPABLE) : (area->flags | LS_MEMORY_UNSWAPPABLE)));
}


ls_result_t
ls_memory_area_set_readable(ls_memory_area_t *const area, const ls_bool readable) {
	return ls_memory_area_set_flags(area, (readable ? (area->flags | LS_MEMORY_READABLE) : (area->flags & ~LS_MEMORY_READABLE)));
}


ls_result_t
ls_memory_area_set_writable(ls_memory_area_t *const area, const ls_bool writeable) {
	return ls_memory_area_set_flags(area, (writeable ? (area->flags | LS_MEMORY_WRITABLE) : (area->flags & ~LS_MEMORY_WRITABLE)));
}


ls_result_t
ls_memory_area_set_executable(ls_memory_area_t *const area, const ls_bool executable) {
	return ls_memory_area_set_flags(area, (executable ? (area->flags | LS_MEMORY_EXECUTABLE) : (area->flags & ~LS_MEMORY_EXECUTABLE)));
}
