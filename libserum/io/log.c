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


#include "./log.h"

#include "../core/memory.h"
#include "../data/time.h"
#include "../runtime/concurrency/thread.h"

#include <string.h>
#include <stdarg.h>
#include <unistd.h>




FILEID("Logging functionality.");




#define GET_LOGPTR(logp)					if ((logp) == NULL) { (logp) = &__global_log; }

#define GET_LOG(logp, post_magic)					\
	const ls_bool_t global = ((logp) == NULL);		\
	GET_LOGPTR((logp));								\
	if (!LS_MAGIC32_VALID((logp)->__flags)) {		\
		if (!global) {								\
			return LS_E_MAGIC;						\
		}											\
		post_magic;									\
		const ls_result_t result = ls_log_init_ex(	\
			(logp),									\
			0,										\
			LS_LOG_LEVEL_DEFAULT,					\
			LS_LOG_STREAM_DEFAULT					\
		);											\
		if (result != LS_E_SUCCESS) {				\
			return result;							\
		}											\
	}




static LS_ATTR_USED ls_log_t __global_log = { 0 };

// YYYY-MM-DD HH:MM:SS LEVEL PID TID
static const char log_prefix[] = "[%04u-%02u-%02u %02u:%02u:%02u %u %05u %05u] ";




ls_result_t
ls_log_init_ex(ls_log_t *restrict log, const uint32_t flags, const ls_log_level_t level, FILE *const restrict std_stream) {
	if (std_stream == NULL) {
		return LS_E_NULL;
	}

	GET_LOGPTR(log);

	if (LS_MAGIC32_VALID(log->__flags)) {
		return LS_E_MAGIC;
	}

	if (LS_FLAG(flags, LS_LOG_MULTI)) {
		// Allocate memory to hold streams for each level.
		//     0: std/fallback
		// <lvl>: null-for-std/<stream>
		FILE **const p = (FILE **const)calloc((LS_LOG_LEVEL_COUNT + 1), sizeof(std_stream));
		if (p == NULL) {
			return LS_E_MEMORY;
		}

		p[0] = std_stream;
		log->__outf = (FILE *const)p;
	} else {
		log->__outf = std_stream;
	}

	if (level > 0 && level <= LS_LOG_LEVEL_COUNT) {
		log->level = level;
	} else {
		log->level = LS_LOG_LEVEL_UNKNOWN;
	}

	log->__flags = LS_MAGIC32_SET(flags);
	return LS_E_SUCCESS;
}


ls_result_t
ls_log_clear_ex(ls_log_t *log, const ls_bool_t close_streams) {
	GET_LOGPTR(log);

	if (!LS_MAGIC32_VALID(log->__flags)) {
		return LS_E_MAGIC;
	}

	const uint32_t flags = log->__flags;

	if (LS_FLAG(flags, LS_LOG_MULTI) && log->__outf != NULL) {
		ls_bool_t errors = false;

		if (close_streams != false) {
			ls_nword_t i;

			FILE
				**const streams = (FILE **const)log->__outf,
				*stream = NULL;

			for (i = (LS_LOG_LEVEL_COUNT + 1); i--;) {
				stream = streams[i];

				if (stream != NULL && (stream != stdin && stream != stdout && stream != stderr)) {
					if (fclose(stream) == 0) {
						streams[i] = NULL;
					} else {
						errors = true;
					}
				}
			}
		}

		if (errors == false) {
			free(log->__outf);
			log->__outf = NULL;
		} else {
			return LS_E_IO_CLOSE;
		}
	}

	log->__flags = 0;
	return LS_E_SUCCESS;
}




ls_result_t
ls_log_set_stream_ex(ls_log_t *restrict log, const ls_log_level_t level, FILE *const restrict stream, const ls_bool_t close_stream) {
	if (level < 0 || level > LS_LOG_LEVEL_COUNT) {
		return LS_E_INVALID;
	}


	// When appropriate, returns an error or initializes the global log.
	GET_LOG(
		log,
		if (!LS_FLAG(log->__flags, LS_LOG_MULTI)) { return LS_E_STATE; }
		if (log->__outf == NULL) { return LS_E_NULL; }
	);


	FILE **const streams = (FILE **const)log->__outf;

	if (close_stream && streams[level] != NULL) {
		FILE *const stream = streams[level];

		if (stream != stdin && stream != stdout && stream != stderr) {
			if (fclose(stream) != 0) {
				return LS_E_IO_CLOSE;
			}
		}
	}

	streams[level] = stream;


	return LS_E_SUCCESS;
}




ls_result_t
ls_log_write(ls_log_t *restrict log, const ls_log_level_t level, const char *const restrict format, ...) {
	if (format == NULL) {
		return LS_E_NULL;
	}


	// When appropriate, returns an error or initializes the global log.
	GET_LOG(log,);


	FILE *stream = NULL;

	if (LS_FLAG(log->__flags, LS_LOG_MULTI)) {
		FILE *const *const streams = (FILE *const *const)log->__outf;

		if (streams[level] != NULL) {
			stream = streams[level];
		} else {
			stream = streams[0];
		}
	} else {
		stream = log->__outf;
	}

	if (stream == NULL) {
		return LS_E_IO_TARGET;
	}


	volatile struct tm tm = { 0 };
	if (ls_localtime_now((struct tm *const)&tm) != LS_E_SUCCESS) {
		return LS_E_FAILURE;
	}


	ls_result_t result = LS_E_SUCCESS;

	const size_t format_length = strlen(format);
	const size_t buffsz = (format_length + sizeof(log_prefix) + LS_EOL_SIZE);


	LS_STACK_ALLOC(char, prefix, buffsz);
#if (LS_VALGRIND)
	memset(prefix, 0, buffsz);
#endif


	int pr = snprintf(
		prefix,
		sizeof(log_prefix),
		log_prefix,
		(1900 + tm.tm_year), (tm.tm_mon + 1), tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec,
		level, getpid(), ls_get_tid()
	);

	if (pr <= 0) {
		result = LS_E_FAILURE;
		goto __cleanup;
	}

	memcpy(&prefix[pr], format, format_length);


	// Transform any CR/NL already in the string to spaces.
	size_t i = 0;
	for (i = buffsz; i--;) {
		if (prefix[i] == '\r' || prefix[i] == '\n') {
			prefix[i] = ' ';
		}
	}

	strcpy(&prefix[pr + format_length], LS_EOL);
	prefix[pr + format_length + LS_EOL_SIZE] = '\0';


	va_list vl;
	va_start(vl, format);
	pr = vfprintf(stream, prefix, vl);
	va_end(vl);

	if (pr <= 0) {
		result = LS_E_IO_WRITE;
	}


__cleanup:
	if (fflush(stream) != 0) {
		result = LS_E_IO_FLUSH;
	}

	LS_STACK_FREE(prefix);
	return result;
}
