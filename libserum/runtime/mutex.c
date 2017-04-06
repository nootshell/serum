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
#include "../debug/log.h"


ls_result_t
ls_mutex_init(ls_mutex_t *mutex) {
	LS_RESULT_CHECK_NULL(mutex, 1);

	if (!(mutex->obj = CreateMutex(NULL, FALSE, NULL))) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_mutex_clear(ls_mutex_t *mutex) {
	LS_RESULT_CHECK_NULL(mutex, 1);

	if (mutex->obj) {
		if (!CloseHandle(mutex->obj)) {
			return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
		}
		mutex->obj = NULL;
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_mutex_lock_ex(ls_mutex_t *mutex, uint32_t timeout) {
	LS_RESULT_CHECK_NULL(mutex, 1);
	LS_RESULT_CHECK_NULL(mutex->obj, 2);

	if (!timeout) {
		timeout = INFINITE;
	}

	DWORD result = WaitForSingleObject(mutex->obj, timeout);
	if (result == WAIT_ABANDONED) {
		ls_log_e("Lock abandoned.");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 1);
	} else if (result == WAIT_FAILED) {
		ls_log_e("Lock failed.");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 2);
	} else if (result == WAIT_TIMEOUT) {
		ls_log_e("Lock timed out.");
		return LS_RESULT_ERROR(LS_RESULT_CODE_TIMEOUT);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_mutex_lock(ls_mutex_t *mutex) {
	return ls_mutex_lock_ex(mutex, 0);
}


ls_result_t
ls_mutex_unlock(ls_mutex_t *mutex) {
	LS_RESULT_CHECK_NULL(mutex, 1);
	LS_RESULT_CHECK_NULL(mutex->obj, 2);

	if (!ReleaseMutex(mutex->obj)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	return LS_RESULT_SUCCESS;
}
