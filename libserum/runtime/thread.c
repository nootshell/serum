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
#include "../debug/log.h"


ID("universal threading");


ls_bool
static inline resume_thread(void *thread) {
	return (ResumeThread(thread) != ~0);
}

ls_bool
static inline suspend_thread(void *thread) {
	return (SuspendThread(thread) != ~0);
}

ls_bool
static inline terminate_thread(void *thread) {
	return (TerminateThread(thread, 0));
}


static DWORD WINAPI __ls_thread_entry(void *param) {
	if (param) {
		ls_thread_t *thread = ((ls_thread_t*)param);
		if (thread->entrypoint && HAS_FLAG(thread->flags, LS_THREAD_STARTED)) {
			thread->flags &= ~LS_THREAD_FINISHED;
			int ret = thread->entrypoint(thread);
			thread->flags |= LS_THREAD_FINISHED;
			return ret;
		} else {
			ls_log_e("Entrypoint or flag missing.");
		}
	} else {
		ls_log_e("Null param.");
	}
	return 1;
}


ls_result_t
ls_thread_init_ex(ls_thread_t *thread, ls_bool(*entrypoint)(ls_thread_t *thread), ls_bool(*stop_handler)(ls_thread_t *thread), size_t stacksize) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_NULL(entrypoint, 2);

	memset(thread, 0, sizeof(*thread));

	thread->stacksize = stacksize;
	thread->entrypoint = entrypoint;
	thread->stop_handler = stop_handler;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_init(ls_thread_t *thread, ls_bool(*entrypoint)(ls_thread_t *thread), ls_bool(*stop_handler)(ls_thread_t *thread)) {
	return ls_thread_init_ex(thread, entrypoint, stop_handler, 0);
}


ls_result_t
ls_thread_clear(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);

	if (thread->thread) {
		if (!CloseHandle(thread->thread)) {
			// error
		}
	}

	memset(thread, 0, sizeof(*thread));

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_start(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_NULL(thread->entrypoint, 3);

	if (thread->thread = CreateThread(
		NULL,
		thread->stacksize,
		__ls_thread_entry,
		thread,
		0,
		&thread->thread_id
	)) {
		thread->flags |= LS_THREAD_STARTED;
		return LS_RESULT_SUCCESS;
	}

	thread->flags &= ~LS_THREAD_STARTED;
	return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
}


ls_result_t
ls_thread_stop(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_NULL(thread->thread, 2);

	if (thread->stop_handler) {
		if (!thread->stop_handler(thread)) {
			return LS_RESULT_ERROR(LS_RESULT_CODE_ABORTED);
		}
	}

	if (!terminate_thread(thread->thread)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_abort(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_NULL(thread->thread, 2);

	if (!terminate_thread(thread->thread)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_MISC);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_resume(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_NULL(thread->thread, 2);

	if (!resume_thread(thread->thread)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	thread->flags &= ~LS_THREAD_SUSPENDED;
	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_suspend(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_NULL(thread->thread, 2);

	if (!suspend_thread(thread->thread)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	thread->flags |= LS_THREAD_SUSPENDED;
	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_join(ls_thread_t *thread) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_NULL(thread->thread, 2);

	if (!HAS_FLAG(thread->flags, LS_THREAD_FINISHED)) {
		while (!HAS_FLAG(thread->flags, LS_THREAD_FINISHED)) {
			ls_sleep_millis(5);
		}
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_set_priority(ls_thread_t *thread, int priority) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_NULL(thread->thread, 2);

	if (!SetThreadPriority(thread->thread, priority)) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_ALLOCATION);
	}
	
	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_thread_get_priority(ls_thread_t *thread, int *out_priority) {
	LS_RESULT_CHECK_NULL(thread, 1);
	LS_RESULT_CHECK_NULL(thread->thread, 2);

	if (!(*out_priority = GetThreadPriority(GetCurrentThread()))) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}

	return LS_RESULT_SUCCESS;
}
