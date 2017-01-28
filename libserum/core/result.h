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

#include "./bool.h"


typedef struct ls_result {		// bitrg
	uint32_t system		: 1;	// 00-01
	uint32_t critical	: 1;	// 01-02
	uint32_t _reserved1	: 6;	// 02-08

	uint32_t code		: 16;	// 08-24

	uint32_t param      : 4;	// 24-28
	uint32_t _reserved2	: 3;	// 28-31
	uint32_t success	: 1;	// 31-32
} ls_result_t;


#define LS_RESULT_CODE_SUCCESS				0	// Success, no error
#define LS_RESULT_CODE_NULL					1	// Null pointer
#define LS_RESULT_CODE_MISC					2	// Misc. error
#define LS_RESULT_CODE_SIZE					3	// Size invalid

#define LS_RESULT(system,critical,code,param,success) ((ls_result_t){(!!(system)),(!!(critical)),0,(code),(param),0,(!!(success))})

#define LS_RESULT_SUCCESS					LS_RESULT(true,false,LS_RESULT_CODE_SUCCESS,0,true)
#define LS_RESULT_ERROR(code)				LS_RESULT(true,false,(code),0,false)
#define LS_RESULT_ERROR_PARAM(code,param)	LS_RESULT(true,false,(code),(param),false)
#define LS_RESULT_ERROR_CRIT(code)			LS_RESULT(true,true,(code),0,false)
#define LS_RESULT_ERROR_CRIT_PARAM(code,param) LS_RESULT(true,true,(code),(param),false)


#endif