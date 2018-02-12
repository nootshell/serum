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


#ifndef __LS_CORE_MEMORY_H




#include "./setup.h"

#include <stdio.h>




#if (LS_GCC || LS_LLVM)
#	define LS_STACK_ALLOC(type, var, sizeb)	type var[(sizeb)]
#	define LS_STACK_FREE(var)
#endif


#ifndef LS_STACK_ALLOC
#	error LS_STACK_ALLOC has not been defined.
#endif

#ifndef LS_STACK_FREE
#	error LS_STACK_FREE has not been defined.
#endif




#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_memory_dump_ex(const void *const src, const size_t size, const ls_nword_t columns, const ls_nword_t columnsize, FILE *const dest, const ls_bool_t ansi);

	static ls_result_t inline ls_memory_dump(const void *const src, const size_t size) {
		return ls_memory_dump_ex(src, size, 16, 1, stdout, true);
	}

#ifdef __cplusplus
}
#endif




#endif
