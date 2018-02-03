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



#include "./thread.h"

#include <string.h>



#if (LS_PTHREADS)
#	define THRAPI_BAD		((void*)0)
#	define THRAPI_GOOD		((void*)1)
#	define THRAPI_RETURN	void*
#elif (LS_WHTREADS)
#	error TODO
#endif



static THRAPI_RETURN __ls_thread_entry(void *param);



FILEID("Threading interface.");



ls_result_t
ls_thread_init(ls_thread_t *const thread, const ls_uint32_t flags) {
	if (thread == NULL) {
		return LS_E_NULL;
	}

	if (LS_MAGIC32_VALID(thread->__flags)) {
		return LS_E_MAGIC;
	}

	if (ls_mutex_init(&thread->__lock) != LS_E_SUCCESS) {
		return LS_E_LOCK;
	}

	thread->__flags = LS_MAGIC32_SET(flags);
	return LS_E_SUCCESS;
}

ls_result_t
ls_thread_clear(ls_thread_t *const thread) {
	if (thread == NULL) {
		return LS_E_NULL;
	}

	if (!LS_MAGIC32_VALID(thread->__flags)) {
		return LS_E_MAGIC;
	}

	if (ls_mutex_clear(&thread->__lock) != LS_E_SUCCESS) {
		return LS_E_LOCK;
	}

#if (LS_PTHREADS)
	/*if (pthread_destroy(&thread->__thread) != 0) {
		return LS_E_FAILURE;
	}*/
#elif (LS_WTHREADS)
#	error TODO
#else
	LS_COMPILER_WARN("Threading unsupported.");
	return LS_E_UNSUPPORTED;
#endif

	thread->__flags = 0;
	return LS_E_SUCCESS;
}


ls_result_t
ls_thread_start_ex(ls_thread_t *const thread, const size_t stacksize) {
	if (thread == NULL) {
		return LS_E_NULL;
	}

	if (!LS_MAGIC32_VALID(thread->__flags)) {
		return LS_E_MAGIC;
	}

#if (LS_PTHREADS)
	pthread_attr_t attr;
	pthread_attr_t *p_attr = NULL;

	if (stacksize > 0) {
		if (pthread_attr_init(&attr) != 0) {
			return LS_E_FAILURE;
		}

		if (pthread_attr_setstacksize(&attr, stacksize) != 0) {
			return LS_E_FAILURE;
		}

		p_attr = &attr;
	}

	if (pthread_create(&thread->__obj, p_attr, &__ls_thread_entry, thread) != 0) {
		return LS_E_FAILURE;
	}
#elif (LS_WTHREADS)
#	error TODO
#else
	return LS_E_UNSUPPORTED;
#endif

	return LS_E_SUCCESS;
}

ls_result_t
ls_thread_start(ls_thread_t *const thread) {
	return ls_thread_start_ex(thread, 0);
}

ls_result_t
ls_thread_stop(ls_thread_t *const thread) {
	if (thread == NULL) {
		return LS_E_NULL;
	}

	if (!LS_MAGIC32_VALID(thread->__flags)) {
		return LS_E_MAGIC;
	}

	return LS_E_UNSUPPORTED;
}


ls_result_t
ls_thread_suspend(ls_thread_t *const thread) {
	return LS_E_UNSUPPORTED;
}

ls_result_t
ls_thread_resume(ls_thread_t *const thread) {
	return LS_E_UNSUPPORTED;
}


static THRAPI_RETURN __ls_thread_entry(void *param) {
	if (param == NULL) {
		return THRAPI_BAD;
	}

	ls_thread_t *thread = param;
	if (!LS_MAGIC32_VALID(thread->__flags)) {
		return THRAPI_BAD;
	}

	// ...

	return THRAPI_GOOD;
}
