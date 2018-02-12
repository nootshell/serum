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


#ifndef __LS_RUNTIME_CONCURRENCY_STATE_H
#define __LS_RUNTIME_CONCURRENCY_STATE_H




#include "../../core/setup.h"

#include "./mutex.h"


#if (LIBSERUM_DOXYGEN)
#	// Doxygen preprocessor.
#elif (LS_PTHREADS)
#	include <pthread.h>
#elif (LS_WTHREADS)
#	error TODO
#else
#	error Unsupported threading API.
#endif




typedef struct ls_state {
#if (LS_PTHREADS)
	pthread_cond_t __cond;
#elif (LS_WTHREADS)
#	error TODO
#endif
	ls_mutex_t __lock;
	ls_nword_t value;
} ls_state_t;




#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_state_init_ex(ls_state_t *const state, const ls_nword_t value);

	static ls_result_t inline ls_state_init(ls_state_t *const state) {
		return ls_state_init_ex(state, 0);
	}

	LSAPI ls_result_t ls_state_clear(ls_state_t *const state);

	LSAPI ls_result_t ls_state_set(ls_state_t *const state, const ls_nword_t value);

	LSAPI ls_result_t ls_state_get(ls_state_t *const restrict state, ls_nword_t *const restrict out_value);

#ifdef __cplusplus
}
#endif




#endif
