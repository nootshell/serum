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

#define FILE_PATH							"core/result.c"

#include "./result.h"
#include "./logging/log.h"


ID("function result value related code");


static const char *strings[] = {
	/* 0x0000 */ "Success, no error",
	/* 0x0001 */ "Null pointer",
	/* 0x0002 */ "Misc. error",
	/* 0x0003 */ "Size invalid",
	/* 0x0004 */ "Access denied",
	/* 0x0005 */ "File/socket descriptor invalid",
	/* 0x0006 */ "Allocation failure",
	/* 0x0007 */ "Early exit",
	/* 0x0008 */ "Lock failure",
	/* 0x0009 */ "Unsupported operation",
	/* 0x000A */ "Data invalid",
	/* 0x000B */ "Function execution within function failed",
	/* 0x000C */ "Close failure",
	/* 0x000D */ "Initialization failure",
	/* 0x000E */ "Operation timed-out",
	/* 0x000F */ "Check failure",
	/* 0x0010 */ "Write failure",
	/* 0x0011 */ "Read failure",
	/* 0x0012 */ "Index invalid",
	/* 0x0013 */ "Operation was aborted",
	/* 0x0014 */ "Invalid type specified",
	/* 0x0015 */ "Invalid state encountered",
	/* 0x0016 */ "Object not found",
	NULL
};
static size_t num_strings = 0;


ls_result_t
__LS_RESULT_PRINT(ls_result_t ret, char const *const func, char const *const file, uint32_t const line) {
	if (ret.code) {
		if (ret.param) {
			ls_log(LS_LOG_INFO, "%08X (" LS_RESULT_PRINTF_FORMAT ") %s (param %u)", (*(uint32_t*)(&ret)), LS_RESULT_PRINTF_PARAMS(ret), ls_result_get_code_string(ret.code), ret.param);
		} else {
			ls_log(LS_LOG_INFO, "%08X (" LS_RESULT_PRINTF_FORMAT ") %s", (*(uint32_t*)(&ret)), LS_RESULT_PRINTF_PARAMS(ret), ls_result_get_code_string(ret.code));
		}
	} else {
		ls_log(LS_LOG_INFO, "%08X (" LS_RESULT_PRINTF_FORMAT ")", (*(uint32_t*)(&ret)), LS_RESULT_PRINTF_PARAMS(ret));
	}
	return ret;
}


const char*
ls_result_get_code_string(uint16_t code) {
	if (!num_strings) {
		for (num_strings = 0; strings[num_strings++];) {
			; // NOP until we encounter NULL
		}
	}

	if (code > num_strings) {
		// > used here instead of >=, strings[code = num_strings] returns NULL anyways.
		return NULL;
	}

	return strings[code];
}
