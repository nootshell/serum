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
*******************************************************************************/



#include <string.h>

#include "../libserum/io/log.h"
#include "../libserum/core/memory.h"
#include "../libserum/data/time.h"
#include "../libserum/runtime/concurrency/thread.h"



int dick = 0;



int thrfun(ls_thread_t *thread) {
	ls_log_write(NULL, LS_LOG_LEVEL_INFO, "Begin.");

	ls_thread_status_t status;

	while (dick == 0) {
		status = ls_thread_status(thread);
		switch (status) {
			case LS_THREAD_STOPPING:
				ls_log_write(NULL, LS_LOG_LEVEL_WARNING, "Thread stop request received.");
				return 1;
			default: break;
		}
	}
	ls_log_write(NULL, LS_LOG_LEVEL_INFO, "End.");
	return 0;
}


void eh_stopped(const ls_event_t *restrict event, void *restrict data) {
	ls_log_write(NULL, LS_LOG_LEVEL_INFO, "Event fired for thread %p", data);
}


int main(int argc, char *argv[], char *env[]) {
	ls_log_write(NULL, LS_LOG_LEVEL_INFO, "Entry.");

	ls_nword_t i;
	ls_result_t result;

#define THREAD_COUNT 10

	ls_thread_t threads[THREAD_COUNT];
	memset(&threads, 0, sizeof(threads));

	for (i = THREAD_COUNT; i--;) {
		if ((result = ls_thread_init(&threads[i], thrfun)) != LS_E_SUCCESS) {
			ls_log_write(NULL, LS_LOG_LEVEL_ERROR, "Failed to initialize thread at index %u: %02X", i, result);
			continue;
		}

		if ((result = ls_event_push(&threads[i].on_stopped, eh_stopped)) != LS_E_SUCCESS) {
			ls_log_write(NULL, LS_LOG_LEVEL_ERROR, "Failed to push event for thread at index %u: %02X", i, result);
			continue;
		}

		if ((result = ls_thread_start(&threads[i])) != LS_E_SUCCESS) {
			ls_log_write(NULL, LS_LOG_LEVEL_ERROR, "Failed to start thread at index %u: %02X", i, result);

			if ((result = ls_thread_clear(&threads[i])) != LS_E_SUCCESS) {
				ls_log_write(NULL, LS_LOG_LEVEL_ERROR, "Failed to clear thread at index %u: %02X", i, result);
			}
		}
	}

	ls_log_write(NULL, LS_LOG_LEVEL_INFO, "Awaiting key press...");
	fgetc(stdin);
	ls_log_write(NULL, LS_LOG_LEVEL_INFO, "Got a key press.");

	for (i = THREAD_COUNT; i--;) {
		if ((result = ls_thread_stop_await(&threads[i])) != LS_E_SUCCESS) {
			if (result != LS_E_STATE) {
				ls_log_write(NULL, LS_LOG_LEVEL_ERROR, "Failed to stop thread at index %u: %02X", i, result);
			}
		}
	}

	fgetc(stdin);
	dick = 1;

	ls_log_write(NULL, LS_LOG_LEVEL_INFO, "Exit.");

	return 0;
}
