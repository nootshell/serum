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


#if (!defined(LS_MEMORY_DESTROY_ITERATIONS))
#	define LS_MEMORY_DESTROY_ITERATIONS		3
#endif


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI const void *const volatile ls_memory_destroy_ex(void *const ptr, const size_t size, ls_nword_t iterations);
	LSAPI const void *const ls_memory_destroy(void *const ptr, const size_t size);
	LSAPI void ls_memory_free_indirect(void **const pptr);

#ifdef __cplusplus
}
#endif


#endif
