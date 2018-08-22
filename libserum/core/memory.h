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
#	//! \hideinitializer \brief Allocates an array on the stack.
#	define LS_STACK_ALLOC(type, var, sizeb)	type var[(sizeb)]
#
#	//! \hideinitializer \brief Deallocates an array from the stack that was previously allocated by `#LS_STACK_ALLOC`.
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
	 * \brief Dumps the memory in the range from \p src to \p src + \p size.
	 * \details Writes the dump to the file specified by \p dest.
	 * \param src The starting address to dump from.
	 * \param size The number of bytes to dump.
	 * \param columns The number of columns to display.
	 * \param columnsize The number of bytes per column.
	 * \param dest A pointer to the `FILE` to dump to.
	 * \param ansi Whether or not to enhance the output using ANSI escape codes.
	 * \return
	 *		`#LS_E_NULL` if \p src or \p dest is `NULL`.					<br/>
	 *		`#LS_E_NOOP` if \p size is less than 1.							<br/>
	 *		`#LS_E_INVALID` if \p columns or \p columnsize is less than 1.	<br/>
	 *		`#LS_E_IO_WRITE` if writing to \p dest fails.					<br/>
	 *		`#LS_E_IO_FLUSH` if flushing to \p dest fails.					<br/>
	 *		`#LS_E_SUCCESS` otherwise.
	 */
	LSAPI ls_result_t ls_memory_dump_ex(const void *const src, const size_t size, const unsigned int columns, const unsigned int columnsize, FILE *const dest, const ls_bool_t ansi);

	/*!
	 * \brief \copybrief ls_memory_dump_ex
	 * \details Writes the dump to `stdout` using 16 single-byte columns.
	 * \param src The starting address to dump from.
	 * \param size The number of bytes to dump.
	 * \return
	 *		`#LS_E_NULL` if \p src or \p dest is `NULL`.					<br/>
	 *		`#LS_E_NOOP` if \p size is less than 1.							<br/>
	 *		`#LS_E_IO_WRITE` if writing to `stdout` fails.					<br/>
	 *		`#LS_E_IO_FLUSH` if flushing to `stdout` fails.					<br/>
	 *		`#LS_E_SUCCESS` otherwise.
	 */
	LSAPI ls_result_t ls_memory_dump(const void *const src, const size_t size);

	/*!
	 * \brief Converts the memory in the range from \p src to \p src + \p size to a string representing the memory as a C array of bytes.
	 * \details Each successful call to this function must be countered by a call to `free` once, to deallocate the allocated memory.
	 * \param src The starting address to dump from.
	 * \param size The number of bytes to dump.
	 * \return
	 *		`NULL` on failure.	<br/>
	 *		A pointer to the newly allocated string otherwise.
	 */
	LSAPI char* ls_memory_to_c_array(const void *const src, const size_t size);

	/*!
	 * \brief Wrapper for the standard `free` function.
	 * \details Unlike standard `free`, this function always returns `NULL` and checks if the passed pointer isn't null before passing it down.
	 * \param target The pointer to the memory to deallocate.
	 * \return `NULL`, always.
	 */
	LSAPI void* ls_memory_free(void *const target);

	/*!
	 * \brief Clears the memory in the range from \p target to \p target + \p size by filling it up with zeroes.
	 * \param target The pointer to the memory to clear.
	 * \param size The number of bytes to clear.
	 */
	LSAPI void ls_memory_clear(volatile void *const target, const size_t size);

	/*!
	 * \brief Clears and deallocates memory by combining calls to `#ls_memory_clear` and `#ls_memory_free`.
	 * \param target The pointer to the memory to clear.
	 * \param size The number of bytes to clear.
	 * \return `NULL`, always.
	 */
	LSAPI void* ls_memory_clear_free(volatile void *const target, const size_t size);

#ifdef __cplusplus
}
#endif




#endif
