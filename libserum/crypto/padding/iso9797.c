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

#define FILE_PATH							"crypto/padding/iso9797.c"

#include <string.h>
#include "./iso9797.h"
#include "../../core/ptrarithmetic.h"


ID("ISO 9797-1 padding methods 1 and 2");


ls_result_t
ls_pad_iso9797_zero_ex(void *const out, void *const in, const size_t inputsz, const size_t outputsz) {
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
ls_pad_iso9797_zero_block(void *const out, void *const in, const size_t inputsz, const int blocksz) {
	return ls_pad_iso9797_zero_ex(out, in, inputsz, ls_pad_iso9797_zero_size(blocksz, inputsz));
}


ls_result_t
ls_pad_iso9797_zero_offset(size_t *const out, const void *const in, const size_t size) {
	if (!out) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}
	if (!in) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 2);
	}
	if (!size) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_SIZE, 1);
	}

	const uint8_t *ptr = in;
	size_t sz = size;

	do {
		if (ptr[--sz]) {
			*out = (sz + 1);
			return LS_RESULT_SUCCESS;
		}
	} while (sz);

#if (LS_ISO9797M1_ALLOW_ALL_ZERO)
	*out = 0;
	return LS_RESULT_SUCCESS;
#else
	return LS_RESULT_ERROR(LS_RESULT_CODE_DATA);
#endif
}


ls_result_t
ls_pad_iso9797_ex(void *const out, void *const in, const size_t inputsz, const size_t outputsz) {
	ls_result_t result = ls_pad_iso9797_zero_ex(out, in, inputsz, outputsz);
	if (!result.success) {
		return result;
	}

	((uint8_t*)LS_SELECT_IO_PTR(out, in))[inputsz] = 0x80;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_pad_iso9797_block(void *const out, void *const in, const size_t inputsz, const int blocksz) {
	return ls_pad_iso9797_ex(out, in, inputsz, ls_pad_iso9797_size(blocksz, inputsz));
}


ls_result_t
ls_pad_iso9797_offset(size_t *const out, const void *const in, const size_t size) {
	size_t offset = 0;

	ls_result_t result;
	if (!(result = ls_pad_iso9797_zero_offset(&offset, in, size)).success) {
		return result;
	}

	if (((uint8_t*)in)[offset - 1] == 0x80) {
		*out = (offset - 1);
		return LS_RESULT_SUCCESS;
	} else {
		return LS_RESULT_ERROR(LS_RESULT_CODE_DATA);
	}
}
