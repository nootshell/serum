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

#define FILE_PATH							"runtime/mutex.c"

#include "./mutex.h"
#include <string.h>


ID("universal mutexes");


ls_result_t
ls_mutex_init(ls_mutex_t *mutex) {
	LS_RESULT_CHECK_NULL(mutex, 1);

#if (LS_USING_PMUTEX)
	if (pthread_mutex_init(&mutex->lock, NULL) != 0) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}
#elif (LS_USING_WNMUTEX)
	if (!(mutex->lock = CreateMutex(NULL, FALSE, NULL))) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}
#else
	LS_COMPILER_WARN(__NO_MUTEX_API_STR);
	return LS_RESULT_UNSUPPORTED;
#endif

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_mutex_clear(ls_mutex_t *mutex) {
	LS_RESULT_CHECK_NULL(mutex, 1);

#if (LS_USING_PMUTEX)
	if (pthread_mutex_destroy(&mutex->lock) != 0) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}
#elif (LS_USING_WNMUTEX)
	if (mutex->lock) {
		if (!CloseHandle(mutex->lock)) {
			return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
		}
		mutex->lock = NULL;
	}
#else
	LS_COMPILER_WARN(__NO_MUTEX_API_STR);
	return LS_RESULT_UNSUPPORTED;
#endif

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_mutex_lock_ex(ls_mutex_t *mutex, uint32_t timeout) {
	LS_RESULT_CHECK_NULL(mutex, 1);

#if (LS_USING_PMUTEX)
	// TODO: timeout
	if (pthread_mutex_lock(&mutex->lock) != 0) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}
#elif (LS_USING_WNMUTEX)
	LS_RESULT_CHECK_NULL(mutex->lock, 2);

	if (!timeout) {
		timeout = INFINITE;
	}

	DWORD result = WaitForSingleObject(mutex->lock, timeout);
	if (result == WAIT_ABANDONED) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 1);
	} else if (result == WAIT_FAILED) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 2);
	} else if (result == WAIT_TIMEOUT) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_TIMEOUT);
	}
#else
	LS_COMPILER_WARN(__NO_MUTEX_API_STR);
	return LS_RESULT_UNSUPPORTED;
#endif

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_mutex_lock(ls_mutex_t *mutex) {
	return ls_mutex_lock_ex(mutex, 0);
}


ls_result_t
ls_mutex_unlock(ls_mutex_t *mutex) {
	LS_RESULT_CHECK_NULL(mutex, 1);

#if (LS_USING_PMUTEX)
	if (pthread_mutex_unlock(&mutex->lock) != 0) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}
#elif (LS_USING_WNMUTEX)
	LS_RESULT_CHECK_NULL(mutex->lock, 2);

	if (!ReleaseMutex(mutex->lock)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}
#else
	LS_COMPILER_WARN(__NO_MUTEX_API_STR);
	return LS_RESULT_UNSUPPORTED;
#endif

	return LS_RESULT_SUCCESS;
}
