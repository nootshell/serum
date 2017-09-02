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

#ifndef __LS_CORE_MEMORY_H
#define __LS_CORE_MEMORY_H


#include "../data/nativeword.h"
#include "./macro.h"
#include "./detect_compiler.h"
#include "./lsapi.h"
#include "./result.h"
#include "../runtime/mutex.h"


#if (LS_MSC)
#	include <malloc.h>
#	define dstacksizeof(name)				__sta_ ## name ## _size__
#	define dstackalloc(name, size)			*name = _malloca((size)); const size_t dstacksizeof(name) = ((size))
#	define dstackfree(name)					_freea(name); name = NULL
#	define stacksizeof(name)				dstacksizeof(name)
#	define stackalloc(name, size)			dstackalloc(name, (size))
#	define stackfree(name)					dstackfree(name)

#	define LS_MEMLOCK(ptr, size)			(VirtualLock((ptr), (size)) != 0)
#	define LS_MEMUNLOCK(ptr, size)			((VirtualUnlock((ptr), (size)) != 0) || (GetLastError() != 153))
#else
#	include <alloca.h>
#	define dstacksizeof(name)				__sta_ ## name ## _size__
#	define dstackalloc(name, size)			*name = alloca((size)); const size_t dstacksizeof(name) = ((size))
#	define dstackfree(name)					name = NULL
#	define stacksizeof(name)				sizeof(name)
#	define stackalloc(name, size)			name[(size)]
#	define stackfree(name)

#	include <sys/mman.h>
#	define LS_MEMLOCK(ptr, size)			(mlock((ptr), (size)) == 0)
#	define LS_MEMUNLOCK(ptr, size)			(munlock((ptr), (size)) == 0)
#endif


#define LS_MEMORY_GUARD_WORD				0xDEADBEEF

#define LS_MEMORY_LOCKED					BIT_1
#define LS_MEMORY_UNSWAPPABLE				LS_MEMORY_LOCKED
#define LS_MEMORY_READABLE					BIT_2
#define LS_MEMORY_WRITABLE					BIT_3
#define LS_MEMORY_EXECUTABLE				BIT_4
#define LS_MEMORY_DESTROY					BIT_5
#define LS_MEMORY_WRAPPED					BIT_6

#define LS_MEMORY_DEFAULT_FLAGS				(LS_MEMORY_READABLE | LS_MEMORY_WRITABLE | LS_MEMORY_DESTROY)


typedef struct ls_memory_area {
	ls_mutex_t perm_mutex;
	void *__location;
	ls_nword_t __guard;
	ls_nword_t flags;
	void *data;
	size_t size;
} ls_memory_area_t;


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI void ls_memory_destroy(void *const ptr, size_t size) LS_ATTR_NONNULL_EX(1);
	LSAPI ls_bool ls_memequ(const void *const LS_RESTRICT cmp1, const void *const LS_RESTRICT cmp2, size_t size) LS_ATTR_PURE LS_ATTR_NONNULL_EX(1, 2);

	LSAPI ls_result_t ls_memory_area_init_ex(ls_memory_area_t *const area, const size_t size, const ls_nword_t flags, const ls_nword_t guard);
	LSAPI ls_result_t ls_memory_area_init(ls_memory_area_t *const area, const size_t size);
	LSAPI ls_result_t ls_memory_area_wrap_ex(ls_memory_area_t *const LS_RESTRICT area, void *const LS_RESTRICT ptr, const size_t size, const ls_nword_t flags);
	LSAPI ls_result_t ls_memory_area_wrap(ls_memory_area_t *const LS_RESTRICT area, void *const LS_RESTRICT ptr, const size_t size);
	LSAPI ls_result_t ls_memory_area_clear(ls_memory_area_t *const area);

	LSAPI ls_result_t ls_memory_area_set_flags(ls_memory_area_t *const area, const ls_nword_t flags);
	LSAPI ls_result_t ls_memory_area_set_swappable(ls_memory_area_t *const area, const ls_bool swappable);		// Default: true
	LSAPI ls_result_t ls_memory_area_set_readable(ls_memory_area_t *const area, const ls_bool readable);		// Default: true
	LSAPI ls_result_t ls_memory_area_set_writable(ls_memory_area_t *const area, const ls_bool writeable);		// Default: true
	LSAPI ls_result_t ls_memory_area_set_executable(ls_memory_area_t *const area, const ls_bool executable);	// Default: false

#ifdef __cplusplus
}
#endif


#endif
