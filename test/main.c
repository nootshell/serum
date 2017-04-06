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

#define FILE_PATH "main.c"

#include <libserum/core/stdincl.h>
#include <libserum/debug/log.h>
#include <libserum/debug/memdump.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <libserum/core/time.h>
#include <libserum/runtime/thread.h>
#include <libserum/runtime/mutex.h>


#define THREAD_COUNT						25


int
thrfunc(ls_thread_t *thread) {
	if (!ls_mutex_lock_ex(thread->tag, 10000).success) {
		return 1;
	}

	printf("Thread with ID %u.", thread->thread_id);
	puts("");

	if (!ls_mutex_unlock(thread->tag).success) {
		return 2;
	}

	return 0;
}


int main(int argc, char *argv[], char *env[]) {
	register unsigned int i;

	ls_mutex_t lock;
	if (!ls_mutex_init(&lock).success) {
		ls_log_e("Mutex init failure.");
		return 1;
	}

	ls_thread_t threads[THREAD_COUNT];
	for (i = THREAD_COUNT; i--;) {
		if (!ls_thread_init_ex(&threads[i], thrfunc, NULL, 1024).success) {
			ls_log_e("Thread init failure.");
			return 2;
		}

		threads[i].tag = &lock;

		if (!ls_thread_start(&threads[i]).success) {
			ls_log_e("Thread start failure.");
			return 3;
		}
	}

	for (i = THREAD_COUNT; i--;) {
		ls_thread_join(&threads[i]);
	}

	ls_mutex_clear(&lock);

	fgetc(stdin);
	return 0;
}
