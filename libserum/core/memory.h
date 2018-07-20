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




#if (LS_GCC || LS_LLVM || LS_TCC)
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

	/*!
	 * \brief ls_memory_dump_ex
	 * \param src
	 * \param size
	 * \param columns
	 * \param columnsize
	 * \param dest
	 * \param ansi
	 * \return
	 */
	LSAPI ls_result_t ls_memory_dump_ex(const void *const src, const size_t size, const ls_nword_t columns, const ls_nword_t columnsize, FILE *const dest, const ls_bool_t ansi);

	/*!
	 * \brief ls_memory_dump
	 * \param src
	 * \param size
	 * \return
	 */
	LSAPI ls_result_t ls_memory_dump(const void *const src, const size_t size);

	/*!
	 * \brief ls_memory_to_c_array
	 * \param data
	 * \param size
	 * \return
	 */
	LSAPI char* ls_memory_to_c_array(const void *const data, const size_t size);

	/*!
	 * \brief ls_memory_free
	 * \param target
	 * \return
	 */
	LSAPI void* ls_memory_free(void *const target);

	/*!
	 * \brief ls_memory_clear
	 * \param target
	 * \param size
	 */
	LSAPI void ls_memory_clear(volatile void *const target, const size_t size);

	/*!
	 * \brief ls_memory_clear_free
	 * \param target
	 * \param size
	 * \return
	 */
	LSAPI void* ls_memory_clear_free(volatile void *const target, const size_t size);

#ifdef __cplusplus
}
#endif




#endif
