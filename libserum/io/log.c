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



FILEID("Logging functionality.");



#define SELECT_LOG(logptr)					\
	if (logptr == NULL) {					\
		logptr = &__global_log;				\
	}



static ls_log_t __global_log = { 0 };



ls_result_t
ls_log_init(ls_log_t *restrict log, const ls_log_level_t level, FILE *const fstd, FILE *const ferr, const ls_uint32_t flags) {
	SELECT_LOG(log);

	if (LS_MAGIC32_VALID(log->flags)) {
		return 1;
	}

	log->fstd = (fstd ? fstd : stdout);
	log->ferr = (ferr ? ferr : stderr);
	log->level = level;
	log->flags = LS_MAGIC32_SET(flags);

	return 0;
}

ls_result_t
ls_log_clear(ls_log_t *log, const ls_uint16_t flags) {
	SELECT_LOG(log);

	if (!LS_MAGIC32_VALID(log->flags)) {
		return 1;
	}

	if (log->fstd) {
		if (LS_FLAG(flags, LS_LOG_CLEAR_CLOSE_STD)) {
			if (fclose(log->fstd) != 0) {
				return 2;
			}
		}
		log->fstd = NULL;
	}

	if (log->ferr) {
		if (LS_FLAG(flags, LS_LOG_CLEAR_CLOSE_ERR)) {
			if (fclose(log->ferr) != 0) {
				return 3;
			}
		}
		log->ferr = NULL;
	}

	log->flags = 0;
	return 0;
}



ls_log_t *
ls_log_alloc(const ls_log_level_t level, FILE *const fstd, FILE *const ferr, const ls_uint32_t flags) {
	errno = 0;

	ls_log_t *mlog = calloc(1, sizeof(ls_log_t));
	if (mlog == NULL) {
		return NULL;
	}

	if (ls_log_init(mlog, level, fstd, ferr, flags) != 0) {
		errno = ECANCELED;
		free(mlog);
		return NULL;
	}

	return mlog;
}

ls_result_t
ls_log_free(ls_log_t *const log, const ls_uint16_t flags) {
	if (log == NULL) {
		return 1;
	}

	ls_result_t result = ls_log_clear(log, flags);
	if (result != 0) {
		return (1 + result);
	}

	free(log);
	return 0;
}



ls_bool_t
ls_log_write_allowed(const ls_log_t *log, const ls_log_level_t target_level) {
	SELECT_LOG(log);

	if (!LS_MAGIC32_VALID(log->flags)) {
		return false;
	}

	return (target_level <= log->level);
}



size_t
ls_log_write(const ls_log_t *restrict log, const char *const restrict buffer, const size_t size, const ls_log_level_t level) {
	if (buffer == NULL || size == 0 || buffer[size] != '\0') {
		return 0;
	}

	SELECT_LOG(log);

	if (!ls_log_write_allowed(log, level)) {
		return 0;
	}

	return 0;
}
