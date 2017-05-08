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

#define FILE_PATH							"data/conversion/floats.c"

#include "./floats.h"
#include "../../core/memory.h"
#include <string.h>



ls_bool
ls_strtold(long double *out, const char *in, size_t size) {
	char *lcp = NULL;
	if (!size) {
		if (out) {
			*out = strtold(in, &lcp);
		} else {
			uintmax_t ph = strtold(in, &lcp);
		}
		return (lcp && *lcp == '\0');
	} else {
		char stackalloc(buffer, (size + 1));
		memcpy(buffer, in, size);
		buffer[size] = 0;
		ls_bool result = ls_strtold(out, buffer, 0);
		stackfree(buffer);
		return result;
	}
}


ls_bool
ls_strtod(double *out, const char *in, size_t size) {
	if (!out) {
		return ls_strtold(NULL, in, size);
	}

	long double buffer;
	ls_bool result = ls_strtold(&buffer, in, size);
	*out = (double)buffer;
	return result;
}


ls_bool
ls_strtof(float *out, const char *in, size_t size) {
	if (!out) {
		return ls_strtold(NULL, in, size);
	}

	long double buffer;
	ls_bool result = ls_strtold(&buffer, in, size);
	*out = (float)buffer;
	return result;
}
