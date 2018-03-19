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




#if (LS_DEBUG)
#	define LS_LOG_LEVEL_DEFAULT				LS_LOG_LEVEL_DEBUG
#	define LS_LOG_STREAM_DEFAULT			stdout
#else
#	define LS_LOG_LEVEL_DEFAULT				LS_LOG_LEVEL_SEVERE
#	define LS_LOG_STREAM_DEFAULT			stderr
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

	LSAPI ls_result_t ls_log_init_ex(ls_log_t *restrict log, const uint32_t flags, const ls_log_level_t level, FILE *const restrict std_stream);

	static ls_result_t inline ls_log_init(ls_log_t *log, const uint32_t flags, const ls_log_level_t level) {
		return ls_log_init_ex(log, flags, level, stdout);
	}

	LSAPI ls_result_t ls_log_clear_ex(ls_log_t *log, const ls_bool_t close_streams);

	static ls_result_t inline ls_log_clear(ls_log_t *log) {
		return ls_log_clear_ex(log, true);
	}

	LSAPI ls_result_t ls_log_level_set(ls_log_t *log, const ls_log_level_t level);

	LSAPI ls_result_t ls_log_set_stream_ex(ls_log_t *restrict log, const ls_log_level_t level, FILE *const restrict stream, const ls_bool_t close_stream);

	static ls_result_t inline ls_log_set_stream(ls_log_t *restrict log, const ls_log_level_t level, FILE *const restrict stream) {
		return ls_log_set_stream_ex(log, level, stream, true);
	}

	LSAPI ls_result_t ls_log_vwrite_ex(ls_log_t *restrict log, const ls_log_level_t level, const ls_bool_t eol, const char *const restrict format, va_list vl);

	static ls_result_t inline ls_log_writeln(ls_log_t *restrict log, const ls_log_level_t level, const char *const restrict format, ...) {
		va_list vl;
		va_start(vl, format);

		const ls_result_t result = ls_log_vwrite_ex(log, level, true, format, vl);

		va_end(vl);


		return result;
	}

	static ls_result_t inline ls_log_write(ls_log_t *restrict log, const ls_log_level_t level, const char *const restrict format, ...) {
		va_list vl;
		va_start(vl, format);

		//const ls_result_t result = ls_log_vwrite_ex(log, level, false, format, vl);
		const ls_result_t result = ls_log_vwrite_ex(log, level, true, format, vl); // < REMOVE AFTER STAMP FIX

		va_end(vl);


		return result;
	}

#ifdef __cplusplus
}
#endif




#endif
