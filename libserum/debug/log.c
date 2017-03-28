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

#define FILE_PATH							"debug/log.c"

#include "./log.h"
#include "../core/time.h"
#include <stdio.h>
#include <inttypes.h>


ID("centralized logging");


static void print_origin(const char *func, const char *file, const uint32_t line) {
	printf("%"PRIu64" %s %s:%- 4"PRIu32" > ", ls_nanos(), func, file, line);
}


// TODO
void
_ls_log(const char *func, const char *file, const uint32_t line, const char *const str) {
	if (!func || !file || !line || !str) {
		puts("");
	} else {
		print_origin(func, file, line);
		puts(str);
	}
}


// TODO
void
_ls_logf(const char *func, const char *file, const uint32_t line, const char *const fmt, ...) {
	if (!func || !file || !line || !fmt) {
		puts("");
	} else {
		va_list vl;
		va_start(vl, fmt);
		print_origin(func, file, line);
		vprintf(fmt, vl);
		puts("");
		va_end(vl);
	}
}
