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
**    TODO FIXME
**
*/

#define FILE_PATH							"data/conversion/integers.c"

#include "./integers.h"
#include "../../core/memory.h"
#include <string.h>



ls_bool
ls_strtoumax(uintmax_t *out, const char *in, size_t size) {
	char *lcp = NULL;
	if (!size) {
		if (out) {
			*out = strtoumax(in, &lcp, 10);
		} else {
			uintmax_t ph = strtoumax(in, &lcp, 10);
		}
		return (lcp && *lcp == '\0');
	} else {
		char stackalloc(buffer, (size + 1));
		memcpy(buffer, in, size);
		buffer[size] = 0;
		ls_bool result = ls_strtoumax(out, buffer, 0);
		stackfree(buffer);
		return result;
	}
}


ls_bool
ls_strtou64(uint64_t *out, const char *in, size_t size) {
	if (!out) {
		return ls_strtoumax(NULL, in, size);
	}

	uintmax_t buffer;
	ls_bool result = ls_strtoumax(&buffer, in, size);
	*out = (uint64_t)buffer;
	return result;
}


ls_bool
ls_strtou32(uint32_t *out, const char *in, size_t size) {
	if (!out) {
		return ls_strtoumax(NULL, in, size);
	}

	uintmax_t buffer;
	ls_bool result = ls_strtoumax(&buffer, in, size);
	*out = (uint32_t)buffer;
	return result;
}


ls_bool
ls_strtou16(uint16_t *out, const char *in, size_t size) {
	if (!out) {
		return ls_strtoumax(NULL, in, size);
	}

	uintmax_t buffer;
	ls_bool result = ls_strtoumax(&buffer, in, size);
	*out = (uint16_t)buffer;
	return result;
}


ls_bool
ls_strtou8(uint8_t *out, const char *in, size_t size) {
	if (!out) {
		return ls_strtoumax(NULL, in, size);
	}

	uintmax_t buffer;
	ls_bool result = ls_strtoumax(&buffer, in, size);
	*out = (uint8_t)buffer;
	return result;
}
