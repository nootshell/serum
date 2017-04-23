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


#define STYLE_TIMESTAMP						LS_ANSI_ESCAPE LS_ANSI_FG_WHITE LS_ANSI_OPT LS_ANSI_DARK LS_ANSI_TERMINATE
#define STYLE_FUNCTION						LS_ANSI_ESCAPE LS_ANSI_FG_WHITE LS_ANSI_OPT LS_ANSI_BRIGHT LS_ANSI_OPT LS_ANSI_ITALIC LS_ANSI_TERMINATE
#define STYLE_FILENAME						STYLE_TIMESTAMP
#define STYLE_SEPARATOR						STYLE_FILENAME
#define STYLE_LINENUMBER					LS_ANSI_ESCAPE LS_ANSI_FG_WHITE LS_ANSI_OPT LS_ANSI_DARK LS_ANSI_TERMINATE
#define STYLE_TERMINATOR					LS_ANSI_ESCAPE LS_ANSI_FG_WHITE LS_ANSI_OPT LS_ANSI_DARK LS_ANSI_TERMINATE
#define STYLE_TEXT							LS_ANSI_RESET

static void print_origin(const char *func, const char *file, const uint32_t line) {
#if (LS_LOG_ORIGIN)
	printf(STYLE_TIMESTAMP "%" PRIu64 " " STYLE_FUNCTION "%- 28s" LS_ANSI_RESET " " STYLE_FILENAME "% 32s" STYLE_SEPARATOR ":" STYLE_LINENUMBER "%- 4" PRIu32 LS_ANSI_RESET " " STYLE_TERMINATOR ">" LS_ANSI_RESET " " STYLE_TEXT, ls_nanos(), func, file, line);
#else
	printf(STYLE_TIMESTAMP "%" PRIu64 " " STYLE_FUNCTION "%- 28s " STYLE_TERMINATOR ">" LS_ANSI_RESET " " STYLE_TEXT, ls_nanos(), func);
#endif
}


// TODO
void
_ls_log(const char *func, const char *file, const uint32_t line, const char *const str) {
	if (!func || !file || !line || !str) {
		puts("");
	} else {
		print_origin(func, file, line);
#if (HAVE_ANSI_SUPPORT)
		printf("%s" LS_ANSI_RESET "\n", str);
#else
		puts(str);
#endif
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
		puts("" LS_ANSI_RESET);
		va_end(vl);
	}
}
