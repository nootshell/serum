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

#define FILE_PATH							"runtime/thread.c"

#include "./thread.h"
#include "../core/time.h"
#include "../core/logging/log.h"
#include <string.h>


ID("universal threading");


ls_bool
static inline resume_thread(ls_thread_t *thread) {
#if (LS_USING_PTHREADS)
	if (pthread_mutex_lock(&thread->p_mutex) != 0) {
		return false;
	}

	thread->flags &= ~LS_THREAD_SUSPEND;

	if (pthread_cond_broadcast(&thread->p_cond) != 0) {
		return false;
	}

	if (pthread_mutex_unlock(&thread->p_mutex) != 0) {
		return false;
	}

	return true;
#elif (LS_USING_WNTHREADS)
	if (ResumeThread(thread->thread) != ~0) {
		thread->flags &= ~LS_THREAD_SUSPEND;
		return true;
	} else {
		return false;
	}
#else
	LS_COMPILER_WARN(__NO_THREAD_API_STR);
	return false;
#endif
}

ls_bool
static inline suspend_thread(ls_thread_t *thread) {
#if (LS_USING_PTHREADS)
	if (pthread_mutex_lock(&thread->p_mutex) != 0) {
		return false;
	}

	thread->flags |= LS_THREAD_SUSPEND;

	if (pthread_mutex_unlock(&thread->p_mutex) != 0) {
		return false;
	}

	return true;
#elif (LS_USING_WNTHREADS)
	if (SuspendThread(thread->thread) != ~0) {
		thread->flags |= LS_THREAD_SUSPEND;
		return true;
	} else {
		return false;
	}
#else
	LS_COMPILER_WARN(__NO_THREAD_API_STR);
	return false;
#endif
}

ls_bool
static inline terminate_thread(ls_thread_t *thread) {
#if (LS_USING_PTHREADS)
	// FIXME: this doesn't truly terminate a thread.
	return (pthread_cancel(thread->thread) != 0);
#elif (LS_USING_WNTHREADS)
	return (TerminateThread(thread->thread, 0));
#else
	LS_COMPILER_WARN(__NO_THREAD_API_STR);
	return false;
#endif
}


ls_result_t
static ls_thread_exec(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);

	if (thread->entrypoint && HAS_FLAG(thread->flags, LS_THREAD_STARTED)) {
		thread->flags &= ~LS_THREAD_FINISHED;
		uint32_t ret = thread->entrypoint(thread);
		thread->flags |= LS_THREAD_FINISHED;

		ls_result_t result = ((ret == LS_RESULT_CODE_SUCCESS) ? LS_RESULT_SUCCESS : LS_RESULT_ERROR(ret));
		return LS_RESULT_INHERITED(result, LS_RESULT_INHERIT_SUCCESS);
	} else {
		ls_log(LS_LOG_ERROR, "Entrypoint or flag missing.");
		return LS_RESULT_ERROR(LS_RESULT_CODE_DATA);
	}

	return LS_RESULT_ERROR(LS_RESULT_CODE_EARLY_EXIT);
}

#if (LS_USING_PTHREADS)
static void* __ls_thread_entry(void *param) {
	return ((void*)(uintptr_t)!ls_thread_exec(param).success); // TODO: appropriate return value
}
#elif (LS_USING_WNTHREADS)
static DWORD WINAPI __ls_thread_entry(void *param) {
	return (!ls_thread_exec(param).success); // TODO: appropriate return value
}
#else
	LS_COMPILER_WARN(__NO_THREAD_API_STR);
#endif


ls_result_t
ls_thread_init_ex(ls_thread_t *thread, ls_thread_entrypoint_t entrypoint, ls_thread_stop_handler_t stop_handler, size_t stacksize) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_NULL(entrypoint, 2);

	memset(thread, 0, sizeof(*thread));

	thread->stacksize = stacksize;
	thread->entrypoint = entrypoint;
	thread->stop_handler = stop_handler;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_init(ls_thread_t *thread, ls_thread_entrypoint_t entrypoint) {
	return ls_thread_init_ex(thread, entrypoint, NULL, 0);
}


ls_result_t
ls_thread_clear(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);

#if (LS_USING_PTHREADS)
	if (thread->thread) {
		if (pthread_join(thread->thread, NULL) != 0) {
			return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
		}
	}
#elif (LS_USING_WNTHREADS)
	if (thread->thread) {
		if (!CloseHandle(thread->thread)) {
			return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
		}
	}
#else
	LS_COMPILER_WARN(__NO_THREAD_API_STR);
	return LS_RESULT_UNSUPPORTED;
#endif

	memset(thread, 0, sizeof(*thread));

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_set_stop_handler(ls_thread_t *thread, ls_thread_stop_handler_t stop_handler) {
	LS_RESULT_CHECK_NULL(thread, 1);

	thread->stop_handler = stop_handler;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_start(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_NULL(thread->entrypoint, 3);

#if (LS_USING_PTHREADS)
	pthread_attr_t attr, *attr_ptr = NULL;
	ls_result_t result = LS_RESULT_SUCCESS;

	if (thread->stacksize) {
		attr_ptr = &attr;
		if (pthread_attr_init(attr_ptr) != 0) {
			return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 1);
		}
		if (pthread_attr_setstacksize(attr_ptr, thread->stacksize) != 0) {
			result =  LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 2);
			goto __cleanup;
		}
	}

	thread->flags |= LS_THREAD_STARTED;
	if (pthread_create(&thread->thread, attr_ptr, __ls_thread_entry, thread) != 0) {
		thread->flags &= ~LS_THREAD_STARTED;
		result = LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 3);
		goto __cleanup;
	}

__cleanup:
	if (attr_ptr) {
		if (pthread_attr_destroy(attr_ptr) != 0) {
			return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 4);
		}
	}

	return result;
#elif (LS_USING_WNTHREADS)
	thread->flags |= LS_THREAD_STARTED;

	thread->thread = CreateThread(
		NULL,
		thread->stacksize,
		__ls_thread_entry,
		thread,
		0,
		&thread->thread_id
	);

	if (!thread->thread) {
		thread->flags &= ~LS_THREAD_STARTED;
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	return LS_RESULT_SUCCESS;
#else
	LS_COMPILER_WARN(__NO_THREAD_API_STR);
	return LS_RESULT_UNSUPPORTED;
#endif
}


ls_result_t
ls_thread_stop(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_THREAD_RUNNING(thread, 1);

	if (thread->stop_handler) {
		if (!thread->stop_handler(thread)) {
			return LS_RESULT_ERROR(LS_RESULT_CODE_ABORTED);
		}
	}

	ls_result_t result = LS_RESULT_SUCCESS;

#if (LS_USING_PTHREADS)
	if (pthread_mutex_lock(&thread->p_mutex) != 0) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 1);
	}

	thread->flags |= LS_THREAD_EXIT;
	if (HAS_FLAG(thread->flags, LS_THREAD_SUSPEND)) {
		if (!ls_thread_resume(thread).success) {
			result = LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 2);
			goto __pthr_mutex_unlock;
		}
	}
	if (!ls_thread_join(thread).success) {
		result = LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 3);
		goto __pthr_mutex_unlock;
	}

__pthr_mutex_unlock:
	if (pthread_mutex_unlock(&thread->p_mutex) != 0) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 4);
	}
#elif (LS_USING_WNTHREADS)
	// TODO
#else
	LS_COMPILER_WARN(__NO_THREAD_API_STR);
	return LS_RESULT_UNSUPPORTED;
#endif

	if (!HAS_FLAG(thread->flags, LS_THREAD_FINISHED)) {
		// Unable to stop thread gracefully - terminate it.
		if (!terminate_thread(thread)) {
			return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
		}
	}

	return result;
}


ls_result_t
ls_thread_abort(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_THREAD_RUNNING(thread, 1);

	if (!terminate_thread(thread)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_resume(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_THREAD_RUNNING(thread, 1);

	if (!resume_thread(thread)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_suspend(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_THREAD_RUNNING(thread, 1);

	if (!suspend_thread(thread)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_join(ls_thread_t *thread) {		// TODO: timeout, sleep?
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_THREAD_RUNNING(thread, 1);

	if (!HAS_FLAG(thread->flags, LS_THREAD_FINISHED)) {
		while (!HAS_FLAG(thread->flags, LS_THREAD_FINISHED)) {
			ls_sleep_micros(20);
		}
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_check_exit(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_THREAD_RUNNING(thread, 1);

	return (HAS_FLAG(thread->flags, LS_THREAD_EXIT) ? LS_RESULT_ERROR(LS_RESULT_CODE_SUCCESS) : LS_RESULT_SUCCESS);
}


ls_result_t
ls_thread_check_suspended(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_THREAD_RUNNING(thread, 1);

#if (LS_USING_PTHREADS)
	if (!HAS_FLAG(thread->flags, LS_THREAD_SUSPEND)) {
		thread->flags &= ~LS_THREAD_SUSPENDED;
		return LS_RESULT_SUCCESS_CODE(LS_RESULT_CODE_EARLY_EXIT);
	}

	if (pthread_mutex_lock(&thread->p_mutex) != 0) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 1);
	}

	thread->flags |= LS_THREAD_SUSPENDED;

	do {
		if (pthread_cond_wait(&thread->p_cond, &thread->p_mutex) != 0) {
			return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 2);
		}
	} while (HAS_FLAG(thread->flags, LS_THREAD_SUSPEND));

	thread->flags &= ~LS_THREAD_SUSPENDED;

	if (pthread_mutex_unlock(&thread->p_mutex) != 0) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 3);
	}

	return LS_RESULT_SUCCESS;
#elif (LS_USING_WNTHREADS)
	return LS_RESULT_SUCCESS_CODE(LS_RESULT_CODE_UNSUPPORTED);
#else
	LS_COMPILER_WARN(__NO_THREAD_API_STR);
	return LS_RESULT_UNSUPPORTED;
#endif
}


ls_result_t
ls_thread_set_priority(ls_thread_t *thread, int priority) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_THREAD_RUNNING(thread, 1);

#if (LS_USING_PTHREADS)
	return LS_RESULT_UNSUPPORTED;
#elif (LS_USING_WNTHREADS)
	if (!SetThreadPriority(thread->thread, priority)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_ALLOCATION);
	}
#else
	LS_COMPILER_WARN(__NO_THREAD_API_STR);
	return LS_RESULT_UNSUPPORTED;
#endif

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_get_priority(ls_thread_t *thread, int *out_priority) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_THREAD_RUNNING(thread, 1);

#if (LS_USING_PTHREADS)
	return LS_RESULT_UNSUPPORTED;
#elif (LS_USING_WNTHREADS)
	if (!(*out_priority = GetThreadPriority(GetCurrentThread()))) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}
#else
	LS_COMPILER_WARN(__NO_THREAD_API_STR);
	return LS_RESULT_UNSUPPORTED;
#endif

	return LS_RESULT_SUCCESS;
}
