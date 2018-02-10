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

#if (LS_LINUX)
#	include <sys/syscall.h>
#	include <unistd.h>
#endif



#if (LS_PTHREADS)
#	define THRAPI_BAD		((void*)0)
#	define THRAPI_GOOD		((void*)1)
#	define THRAPI_RETURN	void*
#elif (LS_WHTREADS)
#	error TODO
#endif



static THRAPI_RETURN __ls_thread_entry(void *param);

static LS_ATTR_THREADLOCAL ls_tid_t __tid = 0;



FILEID("Threading interface.");



ls_tid_t
ls_get_tid() {
#if (LS_LINUX)
	if (__tid == 0) {
		__tid = syscall(SYS_gettid);
	}
	return __tid;
#else
	LS_COMPILER_LOG("");
	return 0;
#endif
}



ls_result_t
ls_thread_init_ex(ls_thread_t *const thread, const ls_uint32_t flags) {
	if (thread == NULL) {
		return LS_E_NULL;
	}

	if (LS_MAGIC32_VALID(thread->__flags)) {
		return LS_E_MAGIC;
	}

	if (ls_mutex_init(&thread->__lock) != LS_E_SUCCESS) {
		return LS_E_LOCK;
	}

	if (ls_state_init(&thread->__state) != LS_E_SUCCESS) {
		return LS_E_STATE;
	}

	thread->__flags = LS_MAGIC32_SET(flags);
	return LS_E_SUCCESS;
}

ls_result_t
ls_thread_clear(ls_thread_t *const thread) {
	if (thread == NULL) {
		ls_debug("NULL encountered.");
		return LS_E_NULL;
	}

	if (!LS_MAGIC32_VALID(thread->__flags)) {
		return LS_E_MAGIC;
	}

	if (ls_mutex_clear(&thread->__lock) != LS_E_SUCCESS) {
		return LS_E_LOCK;
	}

	if (ls_state_clear(&thread->__state) != LS_E_SUCCESS) {
		return LS_E_STATE;
	}

#if (LS_PTHREADS)
	/*if (pthread_destroy(&thread->__thread) != 0) {
		return LS_E_FAILURE;
	}*/
#elif (LS_WTHREADS)
#	error TODO
#else
	LS_COMPILER_LOG("Threading unsupported.");
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

	LS_MUTEX_ACQUIRE_OR_ERROR(&thread->__lock);

	if (ls_state_set(&thread->__state, 1) != LS_E_SUCCESS) {
		goto __ret_failure;
	}

#if (LS_PTHREADS)
	pthread_attr_t attr;
	pthread_attr_t *p_attr = NULL;

	if (stacksize > 0) {
		if (pthread_attr_init(&attr) != 0) {
			goto __ret_failure;
		}

		if (pthread_attr_setstacksize(&attr, stacksize) != 0) {
			goto __ret_failure;
		}

		p_attr = &attr;
	}

	if (pthread_create(&thread->__obj, p_attr, &__ls_thread_entry, thread) != 0) {
		goto __ret_failure;
	}
#elif (LS_WTHREADS)
#	error TODO
#else
	return LS_E_UNSUPPORTED;
#endif

	ls_nword_t state;
	for (;;) {
		if (ls_state_get(&thread->__state, &state) != LS_E_SUCCESS) {
			goto __ret_failure;
		}

		if (state == 0) {
			break;
		}
	}

	LS_MUTEX_RELEASE_OR_ERROR(&thread->__lock);
	return LS_E_SUCCESS;

__ret_failure:
	LS_MUTEX_RELEASE_OR_ERROR(&thread->__lock);
	return LS_E_FAILURE;
}

ls_result_t
ls_thread_stop(ls_thread_t *const thread) {
	if (thread == NULL) {
		ls_debug("NULL encountered.");
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
		ls_debug("NULL encountered.");
		return THRAPI_BAD;
	}

	ls_thread_t *thread = param;
	if (!LS_MAGIC32_VALID(thread->__flags)) {
		return THRAPI_BAD;
	}

	ls_nword_t state;
	if (ls_state_get(&thread->__state, &state) != LS_E_SUCCESS) {
		goto __ret_bad;
	} else if (state != 1) {
		goto __ret_bad;
	}

	// ...

	return THRAPI_GOOD;

__ret_bad:
	ls_state_set(&thread->__state, 0);
	return THRAPI_BAD;
}
