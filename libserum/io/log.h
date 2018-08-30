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


#ifndef __LS_IO_LOG_H
#define __LS_IO_LOG_H




#include "../core/setup.h"

#include "../runtime/concurrency/mutex.h"

#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>




#if (LS_DEBUG)
#	define LS_LOG_LEVEL_DEFAULT				LS_LOG_LEVEL_DEBUG
#else
#	define LS_LOG_LEVEL_DEFAULT				LS_LOG_LEVEL_SEVERE
#endif

#define LS_LOG_MULTI						0x0001




typedef enum ls_log_level {
	LS_LOG_LEVEL_UNKNOWN = 0,
	LS_LOG_LEVEL_SEVERE = 1,
	LS_LOG_LEVEL_ERROR = 2,
	LS_LOG_LEVEL_WARNING = 3,
	LS_LOG_LEVEL_INFO = 4,
	LS_LOG_LEVEL_VERBOSE = 5,
	LS_LOG_LEVEL_DEBUG = 6,

	LS_LOG_LEVEL_COUNT = (LS_LOG_LEVEL_DEBUG)
} ls_log_level_t;

typedef struct ls_log {
	FILE *__outf;

	uint32_t __flags;
	ls_log_level_t level;
	uint32_t __opflags;
	uint32_t __pad;
} ls_log_t;




#ifdef __cplusplus
extern "C" {
#endif

	/*!
	 * \brief Initializes the specified log context.
	 *
	 * \param log The log context to initialize. `NULL` to use the global context.
	 * \param flags The flags to pass down to the context.
	 * \param level The most verbose log level allowed to be logged.
	 * \param std_stream The file stream to log to. If `#LS_LOG_MULTI` is passed to \p flags, this will be the fallback stream if no other stream is found.
	 *
	 * \return
	 *		`#LS_E_NULL` if \p std_stream is `NULL`.
	 *		`#LS_E_MAGIC` if the context has its magic set (already initialized).
	 *		`#LS_E_MEMORY` if memory allocation failed.
	 *		`#LS_E_SUCCESS` otherwise.
	 */
	LSAPI ls_result_t ls_log_init_ex(ls_log_t *restrict log, const uint32_t flags, const ls_log_level_t level, FILE *const restrict std_stream);

	/*!
	 * \brief \copybrief ls_log_init_ex
	 *
	 * \param log The log context to initialize. `NULL` to use the global context.
	 * \param flags The flags to pass down to the context.
	 * \param level The most verbose log level allowed to be logged.
	 *
	 * \return
	 *		Any return value from `#ls_log_init_ex`.
	 */
	LSAPI ls_result_t ls_log_init(ls_log_t *log, const uint32_t flags, const ls_log_level_t level);

	/*!
	 * \brief Clears the specified log context.
	 *
	 * \param log The log context to clear. `NULL` to use the global context.
	 * \param close_streams Whether to close the underlying streams or not.
	 *
	 * \return
	 *		`#LS_E_MAGIC` if the context does not have its magic set (not yet initialized).	\n
	 *		`#LS_E_IO_CLOSE` if a call to `fclose` failed.									\n
	 *		`#LS_E_SUCCESS` otherwise.
	 */
	LSAPI ls_result_t ls_log_clear_ex(ls_log_t *log, const ls_bool_t close_streams);

	/*!
	 * \brief \copybrief ls_log_clear_ex Closes underlying streams.
	 *
	 * \param log The log context to clear. `NULL` to use the global context.
	 *
	 * \return
	 *		Any return value from `#ls_log_clear_ex`.
	 */
	LSAPI ls_result_t ls_log_clear(ls_log_t *log);

	/*!
	 * \brief Sets the log level of the specified log context.
	 *
	 * \param log The log context to set the level of. `NULL` to use the global context.
	 * \param level The most verbose log level allowed to be logged.
	 *
	 * \return
	 *		`#LS_E_INVALID` if \p level is out of range.	\n
	 *		`#LS_E_SUCCESS` otherwise.
	 */
	LSAPI ls_result_t ls_log_level_set(ls_log_t *log, const ls_log_level_t level);

	/*!
	 * \brief Sets the stream for the specified log level on the specified log context.
	 *
	 * Stream applies to the specified log level, and any lower levels that don't have a stream set.
	 *
	 * \param log The log context to configure.
	 * \param level The log level to set the stream of.
	 * \param stream The stream to associate to the log level.
	 * \param close_stream Whether to close an existing stream or not.
	 *
	 * \return
	 *		`#LS_E_INVALID` if \p level is out of range.								\n
	 *		`#LS_E_STATE` if \p log is not initialized with the `#LS_LOG_MULTI` flag.	\n
	 *		`#LS_E_NULL` if an unexpected `NULL` pointer is encountered.				\n
	 *		`#LS_E_IO_CLOSE` if a call to `fclose` failed.								\n
	 *		`#LS_E_SUCCESS` otherwise.
	 */
	LSAPI ls_result_t ls_log_set_stream_ex(ls_log_t *restrict log, const ls_log_level_t level, FILE *const restrict stream, const ls_bool_t close_stream);

	/*!
	 * \brief Sets the stream for the specified log level on the specified log context.
	 *
	 * Stream applies to the specified log level, and any lower levels that don't have a stream set.
	 *
	 * \param log The log context to configure.
	 * \param level The log level to set the stream of.
	 * \param stream The stream to associate to the log level.
	 *
	 * \return
	 *		Any return value from `#ls_log_set_stream_ex`.
	 */
	LSAPI ls_result_t ls_log_set_stream(ls_log_t *restrict log, const ls_log_level_t level, FILE *const restrict stream);

	/*! \cond LS_DOXYGEN_PRIVATE
	 * \brief ls_log_vwrite_ex
	 *
	 * \param log
	 * \param level
	 * \param eol
	 * \param format
	 * \param vl
	 *
	 * \return
	 */
	LSAPI ls_result_t ls_log_vwrite_ex(ls_log_t *restrict log, const ls_log_level_t level, const ls_bool_t eol, const char *const restrict format, va_list vl);

	/*! \endcond
	 * \brief Writes an optionally formatted string to the specified log context.
	 *
	 * \param log The log context to write to.
	 * \param level The level to deem the logging under.
	 * \param format The string to write to the log context, passed to `vfprintf`.
	 *
	 * \return
	 *		`#LS_E_NULL` if \p format is `NULL`.									\n
	 *		`#LS_E_NOOP` if \p level is not allowed to log.							\n
	 *		`#LS_E_IO_TARGET` if no suitable stream was found.						\n
	 *		`#LS_E_FAILURE` if a call to `#ls_localtime_now` or `snprintf` failed.	\n
	 *		`#LS_E_IO_WRITE` if writing to the stream failed.						\n
	 *		`#LS_E_IO_FLUSH` if flushing the stream failed.							\n
	 *		`#LS_E_SUCCESS` otherwise.
	 */
	LSAPI ls_result_t ls_log_writeln(ls_log_t *restrict log, const ls_log_level_t level, const char *const restrict format, ...);

	/*!
	 * \copydoc ls_log_writeln
	 */
	LSAPI ls_result_t ls_log_write(ls_log_t *restrict log, const ls_log_level_t level, const char *const restrict format, ...);

#ifdef __cplusplus
}
#endif




#endif
