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

#ifndef __LS_CORE_RESULT_H
#define __LS_CORE_RESULT_H


#include <stdint.h>
#include <stdlib.h>

#include "./lsapi.h"
#include "./identification.h"
#include "./bool.h"


typedef struct ls_result {		// bit range   max val
	uint32_t system		: 1;	//  00 - 01          1
	uint32_t critical	: 1;	//  01 - 02          1
	uint32_t strict		: 1;	//  02 - 03          1
	uint32_t inherited	: 1;	//  03 - 04          1
	uint32_t inh_depth	: 4;	//  04 - 08         15

	uint32_t code		: 16;	//  08 - 24      65535

	uint32_t param		: 4;	//  24 - 28         15
	uint32_t _reserved	: 3;	//  28 - 31          7
	uint32_t success	: 1;	//  31 - 32          1
} ls_result_t;


#define LS_RESULT_PRINTF_FORMAT				"sys=%c crit=%c strict=%c inherited=%c inh_depth=%u code=%04X param=%u success=%c"
#define LS_RESULT_PRINTF_PARAMS(result)		((result).system ? 'y' : 'n'),((result).critical ? 'y' : 'n'),((result).strict ? 'y' : 'n'),((result).inherited ? 'y' : 'n'),((result).inh_depth),((result).code),((result).param),((result).success ? 'y' : 'n')
#define LS_RESULT_PRINTF(result)			printf(LS_RESULT_PRINTF_FORMAT"\n", LS_RESULT_PRINTF_PARAMS(result))


#define LS_RESULT_CODE_SUCCESS				0x0000	// Success, no error
#define LS_RESULT_CODE_NULL					0x0001	// Null pointer
#define LS_RESULT_CODE_MISC					0x0002	// Misc. error
#define LS_RESULT_CODE_SIZE					0x0003	// Size invalid
#define LS_RESULT_CODE_ACCESS				0x0004	// Access denied
#define LS_RESULT_CODE_DESCRIPTOR			0x0005	// File/socket descriptor invalid
#define LS_RESULT_CODE_ALLOCATION			0x0006	// Allocation failure
#define LS_RESULT_CODE_EARLY_EXIT			0x0007	// Early exit
#define LS_RESULT_CODE_LOCK					0x0008	// Lock failure
#define LS_RESULT_CODE_UNSUPPORTED			0x0009	// Unsupported operation
#define LS_RESULT_CODE_DATA					0x000A	// Data invalid
#define LS_RESULT_CODE_FUNCTION				0x000B	// Function execution within function failed
#define LS_RESULT_CODE_CLOSE				0x000C	// Close failure
#define LS_RESULT_CODE_INITIALIZATION		0x000D	// Initialization failure
#define LS_RESULT_CODE_TIMEOUT				0x000E	// Operation timed-out
#define LS_RESULT_CODE_CHECK				0x000F	// Check failure
#define LS_RESULT_CODE_WRITE				0x0010	// Write failure
#define LS_RESULT_CODE_READ					0x0011	// Read failure
#define LS_RESULT_CODE_INDEX				0x0012	// Index invalid
#define LS_RESULT_CODE_ABORTED				0x0013	// Operation was aborted
#define LS_RESULT_CODE_TYPE					0x0014	// Invalid type specified
#define LS_RESULT_CODE_STATE				0x0015	// Invalid state encountered
#define LS_RESULT_CODE_NOT_FOUND			0x0016	// Object not found


#define LS_RESULT_SA(_system, _critical, _strict, _inherited, _inh_depth, _code, _param, _success)	\
	((ls_result_t){							\
		.system		= (!!(_system)),		\
		.critical	= (!!(_critical)),		\
		.strict		= (!!(_strict)),		\
		.inherited	= (!!(_inherited)),		\
		.inh_depth	= ((_inh_depth) & 0x0F),\
		.code		= ((_code) & 0xFFFF),	\
		.param		= ((_param) & 0x0F),	\
		._reserved	= 0,					\
		.success	= (!!(_success))		\
	})

#if (LS_LOG_RESULTS)
#	include "../debug/log.h"
#	define LS_RESULT(_system, _critical, _strict, _inherited, _inh_depth, _code, _param, _success)	\
	__LS_RESULT_PRINT(LS_RESULT_SA((_system), (_critical), (_strict), (_inherited), (_inh_depth), (_code), (_param), (_success)), __func__, FILE_PATH, __LINE__)
#else
#	define LS_RESULT						LS_RESULT_SA
#endif

#define LS_RESULT_INHERIT_SUCCESS			2

#define LS_RESULT_SUCCESS_CODE(code)		LS_RESULT(true, false, false, false, 0, (code), 0      , true )
#define LS_RESULT_SUCCESS					LS_RESULT_SUCCESS_CODE(LS_RESULT_CODE_SUCCESS)
#define LS_RESULT_ERROR(code)				LS_RESULT(true, false, false, false, 0, (code), 0      , false)
#define LS_RESULT_ERROR_PARAM(code, param)	LS_RESULT(true, false, false, false, 0, (code), (param), false)
#define LS_RESULT_ERROR_CRIT(code)			LS_RESULT(true, true , false, false, 0, (code), 0      , false)
#define LS_RESULT_ERROR_CRIT_PARAM(code, param) \
											LS_RESULT(true, true , false, false, 0, (code), (param), false)
#define LS_RESULT_ERROR_STRICT(code)		LS_RESULT(true, false, true , false, 0, (code), 0      , false)
#define LS_RESULT_ERROR_STRICT_PARAM(code, param) \
											LS_RESULT(true, false, true , false, 0, (code), (param), false)
#define LS_RESULT_INHERITED(result, _success) \
											LS_RESULT((result).system, (result).critical, (result).strict, true, ((result).inh_depth + 1), (result).code, (result).param, (((_success) == 2) ? (result).success : (!!(_success))))
#define LS_RESULT_UNSUPPORTED				LS_RESULT(true, false, false, false, 0, LS_RESULT_CODE_UNSUPPORTED, 0, false)
#define LS_RESULT_UNSUPPORTED_PARAM(param)	LS_RESULT(true, false, false, false, 0, LS_RESULT_CODE_UNSUPPORTED, (param), false)


#define LS_RESULT_CHECK(cond, code, param)	if ((cond)) { return LS_RESULT_ERROR_PARAM((code), (param)); }

#define LS_RESULT_CHECK_NULL(var, param)	LS_RESULT_CHECK(!(var), LS_RESULT_CODE_NULL, (param))
#define LS_RESULT_CHECK_SIZE(var, param)	LS_RESULT_CHECK(!(var), LS_RESULT_CODE_SIZE, (param))
#define LS_RESULT_CHECK_STATE(flags, flag, param) \
											LS_RESULT_CHECK(!HAS_FLAG((flags), (flag)), LS_RESULT_CODE_STATE, (param))

#define LS_RESULT_CHECK_INDEX(var, param, lti, gti)	\
											LS_RESULT_CHECK((((var) < (lti)) || ((var) > (gti))), LS_RESULT_CODE_INDEX, (param))


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t __LS_RESULT_PRINT(ls_result_t ret, char const *const func, char const *const file, uint32_t const line);
	LSAPI const char* ls_result_get_code_string(uint16_t code);

#ifdef __cplusplus
}
#endif


#endif
