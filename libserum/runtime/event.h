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


#ifndef __LS_RUNTIME_EVENT_H
#define __LS_RUNTIME_EVENT_H




#include "../core/setup.h"

#include "./concurrency/mutex.h"




typedef struct ls_event ls_event_t;

typedef void (*ls_event_handler_t)(const ls_event_t *restrict event, void *restrict data);

struct ls_event {
	ls_event_handler_t *__handlers;
	size_t __handler_count;
	size_t __handler_capacity;
	void *tag;
	ls_mutex_t __lock;
	ls_uint32_t __flags;
};




#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_event_init_ex(ls_event_t *const event, const ls_uint32_t flags, const size_t initial_capacity);

	static ls_result_t inline ls_event_init(ls_event_t *const event) {
		return ls_event_init_ex(event, 0, 0);
	}

	LSAPI ls_result_t ls_event_clear(ls_event_t *const event);

	LSAPI ls_result_t ls_event_push(ls_event_t *const event, ls_event_handler_t const handler);

	LSAPI ls_result_t ls_event_pop(ls_event_t *const event, ls_event_handler_t const handler);

	LSAPI ls_result_t ls_event_fire_ex(ls_event_t *const restrict event, void *const restrict data);

	static ls_result_t inline ls_event_fire(ls_event_t *const event) {
		return ls_event_fire_ex(event, NULL);
	}

#ifdef __cplusplus
}
#endif




#endif
