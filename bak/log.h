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

#ifndef __LS_DEBUG_LOG_H
#define __LS_DEBUG_LOG_H


#include "../core/integers.h"
#include "../core/lsapi.h"
#include "../core/identification.h"

#include "../core/errstr.h"

#include <stdarg.h>


#if (LS_ANSI_ESCAPE_SUPPORT && !LS_WINDOWS) // XXX: Not enough support for Windows.
#	define HAVE_ANSI_SUPPORT				1
#	define LS_ANSI_ESCAPE					"\033["
#	define LS_ANSI_TERMINATE				"m"
#	define LS_ANSI_OPT_RESET				"0"
#	define LS_ANSI_RESET					LS_ANSI_ESCAPE LS_ANSI_OPT_RESET LS_ANSI_TERMINATE
#	define LS_ANSI_OPT						";"
#	define _LS_ANSI_BLACK					"0"
#	define _LS_ANSI_RED						"1"
#	define _LS_ANSI_GREEN					"2"
#	define _LS_ANSI_YELLOW					"3"
#	define _LS_ANSI_BLUE					"4"
#	define _LS_ANSI_MAGENTA					"5"
#	define _LS_ANSI_CYAN					"6"
#	define _LS_ANSI_WHITE					"7"
#	define LS_ANSI_FG_BLACK					"3" _LS_ANSI_BLACK
#	define LS_ANSI_FG_RED					"3" _LS_ANSI_RED
#	define LS_ANSI_FG_GREEN					"3" _LS_ANSI_GREEN
#	define LS_ANSI_FG_YELLOW				"3" _LS_ANSI_YELLOW
#	define LS_ANSI_FG_BLUE					"3" _LS_ANSI_BLUE
#	define LS_ANSI_FG_MAGENTA				"3" _LS_ANSI_MAGENTA
#	define LS_ANSI_FG_CYAN					"3" _LS_ANSI_CYAN
#	define LS_ANSI_FG_WHITE					"3" _LS_ANSI_WHITE
#	define LS_ANSI_BG_BLACK					"4" _LS_ANSI_BLACK
#	define LS_ANSI_BG_RED					"4" _LS_ANSI_RED
#	define LS_ANSI_BG_GREEN					"4" _LS_ANSI_GREEN
#	define LS_ANSI_BG_YELLOW				"4" _LS_ANSI_YELLOW
#	define LS_ANSI_BG_BLUE					"4" _LS_ANSI_BLUE
#	define LS_ANSI_BG_MAGENTA				"4" _LS_ANSI_MAGENTA
#	define LS_ANSI_BG_CYAN					"4" _LS_ANSI_CYAN
#	define LS_ANSI_BG_WHITE					"4" _LS_ANSI_WHITE
#	define LS_ANSI_BRIGHT					"1"
#	define LS_ANSI_DARK						"2"
#	define LS_ANSI_ITALIC					"3"
#	define LS_ANSI_UNDERLINE				"4"
#	define LS_ANSI_BLINK					"5"
#	define LS_ANSI_AS_BG					"7"
#	define LS_ANSI_STRIKETHROUGH			"9"
#else
#	define LS_ANSI_ESCAPE
#	define LS_ANSI_TERMINATE
#	define LS_ANSI_RESET
#	define LS_ANSI_OPT
#	define _LS_ANSI_BLACK
#	define _LS_ANSI_RED
#	define _LS_ANSI_GREEN
#	define _LS_ANSI_YELLOW
#	define _LS_ANSI_BLUE
#	define _LS_ANSI_MAGENTA
#	define _LS_ANSI_CYAN
#	define _LS_ANSI_WHITE
#	define LS_ANSI_FG_BLACK
#	define LS_ANSI_FG_RED
#	define LS_ANSI_FG_GREEN
#	define LS_ANSI_FG_YELLOW
#	define LS_ANSI_FG_BLUE
#	define LS_ANSI_FG_MAGENTA
#	define LS_ANSI_FG_CYAN
#	define LS_ANSI_FG_WHITE
#	define LS_ANSI_BG_BLACK
#	define LS_ANSI_BG_RED
#	define LS_ANSI_BG_GREEN
#	define LS_ANSI_BG_YELLOW
#	define LS_ANSI_BG_BLUE
#	define LS_ANSI_BG_MAGENTA
#	define LS_ANSI_BG_CYAN
#	define LS_ANSI_BG_WHITE
#	define LS_ANSI_BRIGHT
#	define LS_ANSI_DARK
#	define LS_ANSI_ITALIC
#	define LS_ANSI_UNDERLINE
#	define LS_ANSI_BLINK
#	define LS_ANSI_AS_BG
#	define LS_ANSI_STRIKETHROUGH
#endif

#if (!defined(HAVE_ANSI_SUPPORT))
#	define HAVE_ANSI_SUPPORT				0
#endif


#if (!defined(FILE_PATH))
#	define FILE_PATH						"unspec_file"
#endif


#define ls_log(s)							_ls_log(__func__, FILE_PATH, __LINE__, (s))
#define ls_logf(fmt, ...)					_ls_logf(__func__, FILE_PATH, __LINE__, (fmt), __VA_ARGS__)


#define ls_log_e(str)						ls_log("[err] " str)
#define ls_logf_e(fmt, ...)					ls_logf("[err] " fmt, __VA_ARGS__)
#define ls_log_w(str)						ls_log("[wrn] " str)
#define ls_logf_w(fmt, ...)					ls_logf("[wrn] " fmt, __VA_ARGS__)
#define ls_log_i(str)						ls_log("[inf] " str)
#define ls_logf_i(fmt, ...)					ls_logf("[inf] " fmt, __VA_ARGS__)
#define ls_log_v(str)						ls_log("[ver] " str)
#define ls_logf_v(fmt, ...)					ls_logf("[ver] " fmt, __VA_ARGS__)

#if (DEBUG)
#	define ls_log_d(s)						ls_log("[dbg] " s)
#	define ls_logf_d(fmt, ...)				ls_logf("[dbg] " fmt, __VA_ARGS__)
#else
#	define ls_log_d(s)
#	define ls_logf_d(fmt, ...)
#endif

#define ls_lognull()						_ls_log(NULL, NULL, 0, NULL);

#define ls_log_ok(str)						ls_log_d("OK " str)
#define ls_logf_ok(fmt, ...)				ls_logf_d("OK " fmt, __VA_ARGS__)
#define ls_log_fail(str)					ls_log_e("FAIL " str)
#define ls_logf_fail(fmt, ...)				ls_logf_e("FAIL " fmt, __VA_ARGS__)


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI void _ls_log(const char *func, const char *file, const uint32_t line, const char *const str);
	LSAPI void _ls_logf(const char *func, const char *file, const uint32_t line, const char *const fmt, ...);

#ifdef __cplusplus
}
#endif


#endif
