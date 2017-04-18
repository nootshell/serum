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

#ifndef __LS_RUNTIME_THREAD_H
#define __LS_RUNTIME_THREAD_H


#define __NO_THREAD_API_STR					"No threading API selected."


#include "../core/stdincl.h"

#if (!LS_WINDOWS)
#	include <pthread.h>
#	define LS_USING_PTHREADS				1
#else
#	define LS_USING_WNTHREADS				1
#endif

#ifndef LS_USING_PTHREADS
#define LS_USING_PTHREADS					0
#endif

#ifndef LS_USING_WNTHREADS
#define LS_USING_WNTHREADS					0
#endif


#define LS_THREAD_FINISHED					BIT_1
#define LS_THREAD_STARTED					BIT_2
#define LS_THREAD_SUSPEND					BIT_3
#define LS_THREAD_SUSPENDED					BIT_4
#define LS_THREAD_EXIT						BIT_5


#define LS_RESULT_CHECK_THREAD_RUNNING(thread, param) \
											LS_RESULT_CHECK((!HAS_FLAG((thread)->flags, LS_THREAD_STARTED) || HAS_FLAG((thread)->flags, LS_THREAD_FINISHED)), LS_RESULT_CODE_STATE, (param))


typedef struct ls_thread ls_thread_t;

typedef uint16_t(*ls_thread_entrypoint_t)(ls_thread_t *thread);
typedef ls_bool(*ls_thread_stop_handler_t)(ls_thread_t *thread);

struct ls_thread {
	ls_thread_entrypoint_t entrypoint;
	ls_thread_stop_handler_t stop_handler;
	void *tag;
	size_t stacksize;
#if (LS_USING_PTHREADS)
	pthread_t thread;
	pthread_mutex_t p_mutex;
	pthread_cond_t p_cond;
#elif (LS_USING_WNTHREADS)
	void *thread;
	uint32_t thread_id;
#else
	LS_COMPILER_WARN(__NO_THREAD_API_STR);
#endif
	uint32_t flags;
};


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_thread_init_ex(ls_thread_t *thread, ls_thread_entrypoint_t entrypoint, ls_thread_stop_handler_t stop_handler, size_t stacksize);
	LSAPI ls_result_t ls_thread_init(ls_thread_t *thread, ls_thread_entrypoint_t entrypoint);
	LSAPI ls_result_t ls_thread_clear(ls_thread_t *thread);

	LSAPI ls_result_t ls_thread_set_stop_handler(ls_thread_t *thread, ls_thread_stop_handler_t stop_handler);

	LSAPI ls_result_t ls_thread_start(ls_thread_t *thread);
	LSAPI ls_result_t ls_thread_stop(ls_thread_t *thread);
	LSAPI ls_result_t ls_thread_abort(ls_thread_t *thread);

	LSAPI ls_result_t ls_thread_resume(ls_thread_t *thread);
	LSAPI ls_result_t ls_thread_suspend(ls_thread_t *thread);
	LSAPI ls_result_t ls_thread_join(ls_thread_t *thread);

	LSAPI ls_result_t ls_thread_check_exit(ls_thread_t *thread);
	LSAPI ls_result_t ls_thread_check_suspended(ls_thread_t *thread);

	LSAPI ls_result_t ls_thread_set_priority(ls_thread_t *thread, int priority);
	LSAPI ls_result_t ls_thread_get_priority(ls_thread_t *thread, int *out_priority);

#ifdef __cplusplus
}
#endif


#endif
