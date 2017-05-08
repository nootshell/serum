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

#ifndef __LS_CORE_MEMDUMP_H
#define __LS_CORE_MEMDUMP_H


#include "../core/stdincl.h"


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI void ls_memdump_ex(const void *const ptr, const size_t size, ls_nword_t columns, ls_nword_t items_per_column);
	LSAPI void ls_memdump(const void *const ptr, const size_t size);
	LSAPI void ls_vmemdump_ex(const void *const LS_RESTRICT ptr, const size_t size, ls_nword_t columns, ls_nword_t items_per_column, const char *const LS_RESTRICT str);
	LSAPI void ls_vmemdump(const void *const LS_RESTRICT ptr, const size_t size, const char *const LS_RESTRICT str);
	LSAPI size_t ls_memdiff_ex(const void *const LS_RESTRICT cmp1, const void *const LS_RESTRICT cmp2, const size_t size, ls_nword_t columns);
	LSAPI size_t ls_memdiff(const void *const LS_RESTRICT cmp1, const void *const LS_RESTRICT cmp2, const size_t size);

#ifdef __cplusplus
}
#endif


#endif
