/******************************************************************************
**                                                                           **
**   The MIT License                                                         **
**                                                                           **
**   Copyright 2017-2018 icecubetray                                         **
**                                                                           **
**   Permission is hereby granted, free of charge, to any person             **
**   obtaining a copy of this software and associated documentation files    **
**   (the "Software"), to deal in the Software without restriction,          **
**   including without limitation the rights to use, copy, modify, merge,    **
**   publish, distribute, sublicense, and/or sell copies of the Software,    **
**   and to permit persons to whom the Software is furnished to do so,       **
**   subject to the following conditions:                                    **
**                                                                           **
**   The above copyright notice and this permission notice shall be          **
**   included in all copies or substantial portions of the Software.         **
**                                                                           **
**   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,         **
**   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF      **
**   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  **
**   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    **
**   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    **
**   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       **
**   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  **
**                                                                           **
******************************************************************************/



#ifndef __LS_CORE_SETUP_RESULT_H
#define __LS_CORE_SETUP_RESULT_H



#include "./types.h"



#define LS_E_SUCCESS						0x00
#define LS_E_FAILURE						0x01
#define LS_E_NULL							0x02
#define LS_E_INVALID						0x03
#define LS_E_UNSUPPORTED					0x04
#define LS_E_MAGIC							0x05
#define LS_E_TIMEOUT						0x06
#define LS_E_ABANDONED						0x07
#define LS_E_SIZE							0x08
#define LS_E_UNINITIALIZED					0x09
#define LS_E_CONVERSION						0x0A
#define LS_E_LOCK							0x0B
#define LS_E_STATE							0x0C
#define LS_E_NOOP							0x0D
#define LS_E_MEMORY							0x0E
#define LS_E_ALREADY						0x0F
#define LS_E_IO_CLOSE						0x10
#define LS_E_IO_TARGET						0x11
#define LS_E_IO_WRITE						0x12
#define LS_E_IO_FLUSH						0x13



typedef ls_nword_t ls_result_t;



#endif
