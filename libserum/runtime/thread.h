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


#include "../core/stdincl.h"


#define LS_THREAD_FINISHED					BIT_1
#define LS_THREAD_STARTED					BIT_2
#define LS_THREAD_SUSPENDED					BIT_3


typedef struct ls_thread ls_thread_t;
struct ls_thread {
	void *thread;
	ls_bool(*entrypoint)(ls_thread_t *thread);
	ls_bool(*stop_handler)(ls_thread_t *thread);
	void *tag;
	size_t stacksize;
	uint32_t flags;
	uint32_t thread_id;
};


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_thread_init_ex(ls_thread_t *thread, ls_bool(*entrypoint)(ls_thread_t *thread), ls_bool(*stop_handler)(ls_thread_t *thread), size_t stacksize);
	LSAPI ls_result_t ls_thread_init(ls_thread_t *thread, ls_bool(*entrypoint)(ls_thread_t *thread), ls_bool(*stop_handler)(ls_thread_t *thread));
	LSAPI ls_result_t ls_thread_clear(ls_thread_t *thread);

	LSAPI ls_result_t ls_thread_start(ls_thread_t *thread);
	LSAPI ls_result_t ls_thread_stop(ls_thread_t *thread);
	LSAPI ls_result_t ls_thread_abort(ls_thread_t *thread);

	LSAPI ls_result_t ls_thread_resume(ls_thread_t *thread);
	LSAPI ls_result_t ls_thread_suspend(ls_thread_t *thread);
	LSAPI ls_result_t ls_thread_join(ls_thread_t *thread);

#ifdef __cplusplus
}
#endif


#endif
