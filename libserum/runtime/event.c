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



ls_result_t
ls_event_init_ex(ls_event_t *const event, const ls_uint32_t flags, const size_t initial_capacity) {
	if (event == NULL) {
		return LS_E_NULL;
	}

	if (LS_MAGIC32_VALID(event->__flags)) {
		return LS_E_MAGIC;
	}

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

	if (event->__handlers) {
		free(event->__handlers);
		event->__handlers = NULL;
	}

	event->__flags = 0;
	return LS_E_SUCCESS;
}



ls_result_t
ls_event_fire_ex(const ls_event_t *const restrict event, void *const restrict data) {
	if (event == NULL) {
		return LS_E_NULL;
	}

	// TODO

	return LS_E_SUCCESS;
}
