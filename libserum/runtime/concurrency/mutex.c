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



#include "./mutex.h"
#include "../../data/time.h"



FILEID("Mutual exclusion lock.");



#if (LS_WTHREADS)
ls_result_t
static inline __WaitForSingleObject(const ls_mutex_t *const mutex, const DWORD timeout) {
	switch (WaitForSingleObject(mutex->lock, timeout)) {
		case WAIT_OBJECT_0:
			return LS_E_SUCCESS;
		case WAIT_ABANDONED:
			return LS_E_ABANDONED;
		case WAIT_TIMEOUT:
			if (timeout && timeout < MAXDWORD) {
				return LS_E_TIMEOUT;
			} else {
				return LS_E_FAILURE;
			}
		default:
			return LS_E_FAILURE;
	}
}
#endif



ls_result_t
ls_mutex_init(ls_mutex_t *const mutex) {
	if (mutex == NULL) {
		return LS_E_NULL;
	}

	if (LS_MAGIC32_VALID(mutex->__flags)) {
		return LS_E_MAGIC;
	}

#if (LS_PTHREADS)
	if (pthread_mutex_init(&mutex->lock, NULL) != 0) {
		return LS_E_FAILURE;
	}
#elif (LS_WTHREADS)
	if ((mutex->lock = CreateMutex(NULL, FALSE, NULL)) == NULL) {
		return LS_E_FAILURE;
	}
#else
	return LS_E_UNSUPPORTED;
#endif

	mutex->__flags = LS_MAGIC32_SET(0);
	return LS_E_SUCCESS;
}

ls_result_t
ls_mutex_clear(ls_mutex_t *const mutex) {
	if (mutex == NULL) {
		return LS_E_NULL;
	}

	if (!LS_MAGIC32_VALID(mutex->__flags)) {
		return LS_E_MAGIC;
	}

#if (LS_PTHREADS)
	if (pthread_mutex_destroy(&mutex->lock) != 0) {
		return LS_E_FAILURE;
	}
#elif (LS_WTHREADS)
	if (mutex->lock) {
		if (CloseHandle(mutex->lock) == FALSE) {
			return LS_E_FAILURE;
		}
		mutex->lock = NULL;
	}
#else
	return LS_E_UNSUPPORTED;
#endif

	mutex->__flags = 0;
	return LS_E_SUCCESS;
}


ls_result_t
ls_mutex_lock(CONST_WTHREADS ls_mutex_t *const mutex) {
	if (mutex == NULL) {
		return LS_E_NULL;
	}

	if (!LS_MAGIC32_VALID(mutex->__flags)) {
		return LS_E_MAGIC;
	}

#if (LS_PTHREADS)
	if (pthread_mutex_lock(&mutex->lock) != 0) {
		return LS_E_FAILURE;
	}
#elif (LS_WTHREADS)
	if (mutex->lock == NULL) {
		return LS_E_UNINITIALIZED;
	}

	return __WaitForSingleObject(mutex, INFINITE);
#else
	return LS_E_UNSUPPORTED;
#endif

	return LS_E_SUCCESS;
}


ls_result_t
ls_mutex_timedlock(CONST_WTHREADS ls_mutex_t *const mutex, const struct timespec timeout) {
#if (!LS_WTHREADS)
	// WTHREADS already checks these in ls_timedlock_millis

	if (mutex == NULL) {
		return LS_E_NULL;
	}

	if (!LS_MAGIC32_VALID(mutex->__flags)) {
		return LS_E_MAGIC;
	}
#endif

	if (timeout.tv_sec == 0 && timeout.tv_nsec == 0) {
		return LS_E_INVALID;
	}

#if (LS_PTHREADS)
	if (pthread_mutex_timedlock(&mutex->lock, &timeout) != 0) {
		return LS_E_FAILURE;
	}
#elif (LS_WTHREADS)
	ls_uint64_t millis = 0;
	if (ls_timespec_to_millis(&timeout, &millis) != LS_E_SUCCESS) {
		return LS_E_CONVERSION;
	}

	return ls_mutex_timedlock_millis(mutex, millis);
#else
	return LS_E_UNSUPPORTED;
#endif

	return LS_E_SUCCESS;
}


ls_result_t
ls_mutex_timedlock_millis(CONST_WTHREADS ls_mutex_t *const mutex, const ls_uint64_t timeout_millis) {
	if (mutex == NULL) {
		return LS_E_NULL;
	}

	if (timeout_millis == 0) {
		return LS_E_INVALID;
	}

	if (!LS_MAGIC32_VALID(mutex->__flags)) {
		return LS_E_MAGIC;
	}

#if (LS_PTHREADS)
	struct timespec ts = { 0 };
	if (ls_millis_to_timespec(timeout_millis, &ts) != LS_E_SUCCESS) {
		return LS_E_CONVERSION;
	}

	if (pthread_mutex_timedlock(&mutex->lock, &ts) != 0) {
		return LS_E_FAILURE;
	}
#elif (LS_WTHREADS)
	if (mutex->lock == NULL) {
		return LS_E_UNINITIALIZED;
	}

	if (timeout_millis >= MAXDWORD) {
		return LS_E_SIZE;
	}

	return __WaitForSingleObject(mutex, (const DWORD)timeout_millis);
#else
	return LS_E_UNSUPPORTED;
#endif

	return LS_E_SUCCESS;
}


ls_result_t
ls_mutex_unlock(CONST_WTHREADS ls_mutex_t *const mutex) {
	if (mutex == NULL) {
		return LS_E_NULL;
	}

	if (!LS_MAGIC32_VALID(mutex->__flags)) {
		return LS_E_MAGIC;
	}

#if (LS_PTHREADS)
	if (pthread_mutex_unlock(&mutex->lock) != 0) {
		return LS_E_FAILURE;
	}
#elif (LS_WTHREADS)
	if (mutex->lock == NULL) {
		return LS_E_UNINITIALIZED;
	}

	if (ReleaseMutex(mutex->lock) == 0) {
		return LS_E_FAILURE;
	}
#else
	return LS_E_UNSUPPORTED;
#endif

	return LS_E_SUCCESS;
}
