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

#define FILE_PATH							"core/varsize.c"

#include "./varsize.h"
#include <string.h>


ID("variable size integer storage");


ls_result_t
ls_varsize_get_bytes(void *const out, size_t *const out_size, ls_vs_value_t value) {
	LS_RESULT_CHECK_NULL(out, 1);
	LS_RESULT_CHECK_NULL(out_size, 2);

	if (!value) {
		*((uint8_t*)out) = 0;
		*out_size = 1;
		return LS_RESULT_SUCCESS;
	}

	size_t sz = 0;
	uint8_t *ptr = (((uint8_t*)out) - 1);
	do {
		*(++ptr) = (uint8_t)((value & 0x7F) | 0x80); // 0x80 = MSB = further processing indicator
		++sz;
	} while (value >>= 7);

	// Remove the MSB to indicate the end.
	*ptr = (*ptr & 0x7F);

	*out_size = sz;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_varsize_get_value(ls_vs_value_t *const out, const void *const in, size_t max_size) {
	LS_RESULT_CHECK_NULL(out, 1);
	LS_RESULT_CHECK_NULL(in, 2);
	LS_RESULT_CHECK_SIZE(max_size, 1);

	uint_fast16_t bpos = 0;
	ls_vs_value_t value = 0;
	const uint8_t *ptr = in;
	for (; max_size--;) {
		value |= (((ls_vs_value_t)(*ptr & 0x7F)) << (7 * bpos++));
		if (!HAS_FLAG(*(ptr++), 0x80)) {
			*out = value;
			return LS_RESULT_SUCCESS;
		}
	}

	*out = value;

	return LS_RESULT_ERROR(LS_RESULT_CODE_EARLY_EXIT);
}
