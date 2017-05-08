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

#ifndef __LS_CORE_TIME_H
#define __LS_CORE_TIME_H


#include "./stdincl.h"


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI uint64_t ls_rdtsc();

	LSAPI uint64_t ls_nanos();
#define ls_micros()							(ls_nanos() / 1000)
#define ls_millis()							(ls_nanos() / 1000000)
#define ls_secs()							(ls_nanos() / 1000000000)

	LSAPI void ls_sleep_nanos(const uint64_t nanos);
#define ls_sleep_micros(x)					ls_sleep_nanos((x) * 1000)
#define ls_sleep_millis(x)					ls_sleep_nanos((x) * 1000000)
#define ls_sleep_secs(x)					ls_sleep_nanos((x) * 1000000000)

#ifdef __cplusplus
}
#endif


#endif
