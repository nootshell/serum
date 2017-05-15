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

#ifndef __LS_DEBUG___SELFTEST_H
#define __LS_DEBUG___SELFTEST_H


#if (LS_SELFTEST_VERBOSE)
#	include "../core/logging/log.h"

#	define COLOR_TEST_PASSED				LS_ANSI_1(LS_ANSI_FG_LIME)
#	define COLOR_TEST_FAILED				LS_ANSI_1(LS_ANSI_FG_BRIGHT_RED)
#	define COLOR_TEST_TEXT_GENERIC			LS_ANSI_1(LS_ANSI_FG_WHITE)
#	define COLOR_TEST_TEXT_DATA				LS_ANSI_1(LS_ANSI_FG_PINK)
#	define COLOR_TEST_TEXT_VECTOR			LS_ANSI_1(LS_ANSI_FG_TEAL)
#	define COLOR_TEST_TEXT_SUB				COLOR_TEST_TEXT_VECTOR
#	define COLOR_TEST_ARROW					COLOR_TEST_TEXT_GENERIC
#	define COLOR_TEST_ARROW_BODY			LS_ANSI_1(LS_ANSI_FG_CYAN)

#	define START_TEST(desc)					ls_log(LS_LOG_INFO, COLOR_TEST_TEXT_GENERIC "Self-testing " desc "..." LS_ANSI_RESET)
#	define END_TEST(desc, result)			ls_log(LS_LOG_INFO, COLOR_TEST_TEXT_GENERIC "Self-test ended for " desc ": " result LS_ANSI_RESET)
#	define END_TEST_PASSED(desc)			END_TEST(desc, COLOR_TEST_PASSED "passed" LS_ANSI_RESET)
#	define END_TEST_FAILED(desc)			END_TEST(desc, COLOR_TEST_FAILED "failed" LS_ANSI_RESET)
#	define START_VECTOR(iter, str)			ls_log(LS_LOG_VERBOSE, "  " COLOR_TEST_TEXT_VECTOR "Vector %u" LS_ANSI_RESET ": " COLOR_TEST_TEXT_DATA "\"%s\"" LS_ANSI_RESET, ((iter) + 1), (str))
#	define TEST_SUB_RESULT(desc, result)	ls_log(LS_LOG_VERBOSE, "    " COLOR_TEST_TEXT_SUB desc LS_ANSI_RESET ": " result)
#	define TEST_SUB_PASSED(desc)			TEST_SUB_RESULT(desc, COLOR_TEST_PASSED "passed" LS_ANSI_RESET)
#	define TEST_SUB_FAILED(desc)			TEST_SUB_RESULT(desc, COLOR_TEST_FAILED "failed " COLOR_TEST_ARROW "<----- [ " COLOR_TEST_ARROW_BODY ">:c" COLOR_TEST_ARROW" ]" LS_ANSI_RESET)
#else
#	define START_TEST(x)
#	define END_TEST(x, y)
#	define END_TEST_PASSED(desc)
#	define END_TEST_FAILED(desc)
#	define START_VECTOR(x, y)
#	define TEST_SUB_RESULT(x, y)
#	define TEST_SUB_PASSED(x)
#	define TEST_SUB_FAILED(x)
#endif


#endif
