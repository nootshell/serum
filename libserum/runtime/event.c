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



#include "./event.h"

#include "../collections/utilities.h"



FILEID("Simple event interface.");



ls_result_t
ls_event_init_ex(ls_event_t *const event, const ls_uint32_t flags, const size_t initial_capacity) {
	if (event == NULL) {
		return LS_E_NULL;
	}

	if (LS_MAGIC32_VALID(event->__flags)) {
		return LS_E_MAGIC;
	}

	if (ls_mutex_init(&event->__lock) != LS_E_SUCCESS) {
		return LS_E_LOCK;
	}

	LS_MUTEX_ACQUIRE_OR_ERROR(&event->__lock);

	if (initial_capacity > 0) {
		event->__handlers = calloc(
			(event->__handler_capacity = initial_capacity),
			sizeof(*event->__handlers)
		);
	} else {
		event->__handlers = malloc(
			(event->__handler_capacity = 0)
		);
	}

	event->__flags = LS_MAGIC32_SET(flags);

	LS_MUTEX_RELEASE_OR_ERROR(&event->__lock);

	return LS_E_SUCCESS;
}

ls_result_t
ls_event_clear(ls_event_t *const event) {
	if (event == NULL) {
		return LS_E_NULL;
	}

	if (!LS_MAGIC32_VALID(event->__flags)) {
		return LS_E_MAGIC;
	}

	LS_MUTEX_ACQUIRE_OR_ERROR(&event->__lock);
	if (ls_mutex_clear(&event->__lock) != LS_E_SUCCESS) {
		return LS_E_LOCK;
	}

	if (event->__handlers) {
		free(event->__handlers);
		event->__handlers = NULL;
	}

	event->__flags = 0;
	return LS_E_SUCCESS;
}



ls_result_t
ls_event_push(ls_event_t *const event, ls_event_handler_t const handler) {
	if (event == NULL || handler == NULL) {
		return LS_E_NULL;
	}

	if (!LS_MAGIC32_VALID(event->__flags)) {
		return LS_E_MAGIC;
	}


	LS_MUTEX_ACQUIRE_OR_ERROR(&event->__lock);


	const size_t cap = event->__handler_capacity;
	ls_event_handler_t *handlers = event->__handlers;
	ls_event_handler_t *hp = NULL;


	// Check if we already have the handler in our collection.
	hp = ls_pcollection_rfind((ls_pcollection_t)handlers, handler, cap);
	if (hp != NULL) {
		return LS_E_ALREADY;
	}


	size_t nhc = (event->__handler_count + 1);

	if (nhc > cap) {
		handlers = realloc(handlers, (sizeof(*handlers) * nhc));
		if (handlers == NULL) {
			LS_MUTEX_RELEASE_OR_ERROR(&event->__lock);
			return LS_E_MEMORY;
		}

		event->__handlers = handlers;
		event->__handler_capacity = nhc;

		handlers[event->__handler_count] = handler;
	} else {
		hp = ls_pcollection_rfind((ls_pcollection_t)handlers, NULL, cap);
		if (hp == NULL) {
			return LS_E_FAILURE;
		}

		*hp = handler;
	}


	LS_MUTEX_RELEASE_OR_ERROR(&event->__lock);

	return LS_E_SUCCESS;
}

ls_result_t
ls_event_pop(ls_event_t *const event, ls_event_handler_t const handler) {
	if (event == NULL) {
		return LS_E_NULL;
	}

	return LS_E_UNSUPPORTED;
}



ls_result_t
ls_event_fire_ex(ls_event_t *const restrict event, void *const restrict data) {
	if (event == NULL) {
		return LS_E_NULL;
	}

	if (!LS_MAGIC32_VALID(event->__flags)) {
		return LS_E_MAGIC;
	}

	if (event->__handler_count == 0) {
		return LS_E_NOOP;
	}

	LS_MUTEX_ACQUIRE_OR_ERROR(&event->__lock);

	ls_event_handler_t handler;
	size_t hc = event->__handler_count;
	for (; hc--;) {
		handler = event->__handlers[hc];

		if (handler != NULL) {
			handler(event, data);
		}
	}

	LS_MUTEX_RELEASE_OR_ERROR(&event->__lock);

	return LS_E_SUCCESS;
}
