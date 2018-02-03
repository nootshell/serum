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

#if (LIBSERUM_DOXYGEN)
#	// Doxygen preprocessor.
#elif (LS_PTHREADS)
#	include <pthread.h>
#elif (!LS_WTHREADS)
#	error Unsupported threading API.
#endif



typedef enum ls_thread_status {
	LS_THREAD_UNKNOWN = 0xFFFF,
	LS_THREAD_RUNNING = 0x0001,
	LS_THREAD_SUSPENDED = 0x0002
} ls_thread_status_t;

typedef struct ls_thread {
#if (LIBSERUM_DOXYGEN)
	platform_specific thr_objs;
#elif (LS_PTHREADS)
	pthread_t __obj;
#elif (LS_WTHREADS)
#	error TODO
#endif
	ls_mutex_t __lock;
	ls_uint32_t __flags;
	ls_thread_status_t status;
} ls_thread_t;



#endif
