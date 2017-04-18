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
#	include "./log.h"

#	define START_TEST(desc)					ls_log("\033[37;1mSelf-testing " desc "...\033[0m")
#	define END_TEST(desc, result)			ls_log("\033[37;1mSelf-test ended for " desc ": " result "\033[0m\n")
#	define START_VECTOR(iter, str)			ls_logf("  \033[36mVector %u\033[0m: \033[35;1m\"%s\"\033[0m", ((iter) + 1), (str))
#	define TEST_SUB_RESULT(desc, result)	ls_log("    \033[36m" desc "\033[0m: " result)
#	define TEST_SUB_PASSED(desc)			TEST_SUB_RESULT(desc, "\033[32;1mpassed\033[0m")
#	define TEST_SUB_FAILED(desc)			TEST_SUB_RESULT(desc, "\033[31;1mfailed \033[37;1m<----- [ \033[36;1m>:c\033[37;1m ]\033[0m")
#else
#	define START_TEST(x)
#	define END_TEST(x, y)
#	define START_VECTOR(x, y)
#	define TEST_SUB_RESULT(x, y)
#	define TEST_SUB_PASSED(x)
#	define TEST_SUB_FAILED(x)
#endif


#endif
