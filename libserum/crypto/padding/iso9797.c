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

#define __RFILE__							"crypto/padding/iso9797.c"

#include "./iso9797.h"

#include <string.h>

#include "../../core/ptrarithmetic.h"


ls_result_t
ls_pad_iso9797_zero(void *out, void *in, const size_t inputsz, const size_t outputsz) {
	if (!in) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}
	if (!outputsz || (outputsz <= inputsz)) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_SIZE, 1);
	}
#if (!defined(LS_ALLOW_PAD_INPUTSZ_ZERO) || !LS_ALLOW_PAD_INPUTSZ_ZERO)
	if (!inputsz) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_SIZE, 2);
	}
#endif

	memset((LS_SELECT_IO_PTR_WCPY(out, in, inputsz) + inputsz), 0, (outputsz - inputsz));

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_pad_iso9797(void *out, void *in, const size_t inputsz, const size_t outputsz) {
    ls_result_t result = ls_pad_iso9797_zero(out, in, inputsz, outputsz);
    if (!result.success) {
		return result;
    }

    ((uint8_t*)LS_SELECT_IO_PTR(out, in))[inputsz] = 0x80;

    return LS_RESULT_SUCCESS;
}


