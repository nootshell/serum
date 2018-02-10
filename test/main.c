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


int main(int argc, char *argv[], char *env[]) {
	ls_log_t log = { 0 };
	//ls_memory_dump(&log, sizeof(log));

	ls_log_init(&log, LS_LOG_MULTI, LS_LOG_LEVEL_DEBUG);
	//ls_memory_dump(&log, sizeof(log));

	ls_log_write(&log, LS_LOG_LEVEL_INFO, "%s%s - %s %s! - %i %llu", "ab", "cd", "hello", "world", 1, ls_time_nanos());
	ls_log_write(&log, LS_LOG_LEVEL_INFO, "%s%s - %s %s! - %i", "ab", "cd", "hello", "world", 1);
	ls_log_write(&log, LS_LOG_LEVEL_INFO, "%s%s - %s %s! - %i", "ab", "cd", "hello", "world", 1);
	ls_log_write(&log, LS_LOG_LEVEL_INFO, "%s%s - %s %s! - %i", "ab", "cd", "hello", "world", 1);

	ls_log_clear(&log);
	//ls_memory_dump(&log, sizeof(log));

	return 0;
}
