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


#ifndef __LS_RUNTIME_CONCURRENCY_THREAD_H
#define __LS_RUNTIME_CONCURRENCY_THREAD_H




#include "../../core/setup.h"

#include "./mutex.h"
#include "./state.h"
#include "../event.h"


#if (LS_DOXYGEN)
#	// Doxygen preprocessor.
#elif (LS_PTHREADS)
#	include <pthread.h>
#elif (!LS_WTHREADS)
#	error Unsupported threading API.
#endif




typedef int ls_tid_t;

typedef enum ls_thread_status {
	LS_THREAD_ERROR = 0,
	LS_THREAD_INITIALIZING = 1,
	LS_THREAD_RUNNING = 2,
	LS_THREAD_STOPPING = 3,
	LS_THREAD_STOPPED = 4
} ls_thread_status_t;


typedef struct ls_thread ls_thread_t;

typedef int (*ls_thread_entry_t)(ls_thread_t *thread);

struct ls_thread {
	ls_thread_entry_t entrypoint;
	void *tag;
	ls_event_t on_stopped;
#if (LS_DOXYGEN)
	platform_specific thr_objs;
#elif (LS_PTHREADS)
	pthread_t __obj;
#elif (LS_WTHREADS)
#	error TODO
#endif
	ls_mutex_t __lock;
	ls_state_t __state;
	uint32_t __flags;
	int exit_code;
};




#ifdef __cplusplus
extern "C" {
#endif

	/*!
	 * \brief ls_get_tid
	 * \return
	 */
	LSAPI ls_tid_t ls_get_tid(void);

	/*!
	 * \brief ls_thread_init_ex
	 * \param thread
	 * \param entrypoint
	 * \param flags
	 * \return
	 */
	LSAPI ls_result_t ls_thread_init_ex(ls_thread_t *const thread, const ls_thread_entry_t entrypoint, const uint32_t flags);

	/*!
	 * \brief ls_thread_init
	 * \param thread
	 * \param entrypoint
	 * \return
	 */
	LSAPI ls_result_t ls_thread_init(ls_thread_t *const thread, const ls_thread_entry_t entrypoint);

	/*!
	 * \brief ls_thread_clear
	 * \param thread
	 * \return
	 */
	LSAPI ls_result_t ls_thread_clear(ls_thread_t *const thread);

	/*!
	 * \brief ls_thread_start_ex
	 * \param thread
	 * \param stacksize
	 * \return
	 */
	LSAPI ls_result_t ls_thread_start_ex(ls_thread_t *const thread, const size_t stacksize);

	/*!
	 * \brief ls_thread_start
	 * \param thread
	 * \return
	 */
	LSAPI ls_result_t ls_thread_start(ls_thread_t *const thread);

	/*!
	 * \brief ls_thread_start_ex_await
	 * \param thread
	 * \param stacksize
	 * \return
	 */
	LSAPI ls_result_t ls_thread_start_ex_await(ls_thread_t *const thread, const size_t stacksize);

	/*!
	 * \brief ls_thread_start_await
	 * \param thread
	 * \return
	 */
	LSAPI ls_result_t ls_thread_start_await(ls_thread_t *const thread);

	/*!
	 * \brief ls_thread_stop
	 * \param thread
	 * \return
	 */
	LSAPI ls_result_t ls_thread_stop(ls_thread_t *const thread);

	/*!
	 * \brief ls_thread_stop_await
	 * \param thread
	 * \return
	 */
	LSAPI ls_result_t ls_thread_stop_await(ls_thread_t *const thread);

	/*!
	 * \brief ls_thread_suspend
	 * \param thread
	 * \return
	 */
	LSAPI ls_result_t ls_thread_suspend(ls_thread_t *const thread);

	/*!
	 * \brief ls_thread_resume
	 * \param thread
	 * \return
	 */
	LSAPI ls_result_t ls_thread_resume(ls_thread_t *const thread);

	/*!
	 * \brief ls_thread_status
	 * \param thread
	 * \return
	 */
	LSAPI ls_thread_status_t ls_thread_status(ls_thread_t *const thread);

#ifdef __cplusplus
}
#endif




#endif
