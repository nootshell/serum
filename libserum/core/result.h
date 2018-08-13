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


#ifndef __LS_CORE_RESULT_H
#define __LS_CORE_RESULT_H




#include "./setup/lsapi.h"
#include "./setup/types.h"




/*! \brief Successful. */
#define LS_E_SUCCESS						0x00

/*! \brief Failure. */
#define LS_E_FAILURE						0x01

/*! \brief Null-pointer encountered. */
#define LS_E_NULL							0x02

/*! \brief Parameter or state/data invalid. */
#define LS_E_INVALID						0x03

/*! \brief Operation not supported (not yet implemented). */
#define LS_E_UNSUPPORTED					0x04

/*! \brief Magic value found (or not) where inappropriate. */
#define LS_E_MAGIC							0x05

/*! \brief Operation timed out. */
#define LS_E_TIMEOUT						0x06

/*! \brief Mutex/operation was abandoned. */
#define LS_E_ABANDONED						0x07

/*! \brief Size parameter/data invalid. */
#define LS_E_SIZE							0x08

/*! \brief Object is in an uninitialized state. */
#define LS_E_UNINITIALIZED					0x09

/*! \brief Conversion failure. */
#define LS_E_CONVERSION						0x0A

/*! \brief Mutex-related failure. */
#define LS_E_LOCK							0x0B

/*! \brief Object is in an invalid state. */
#define LS_E_STATE							0x0C

/*! \brief The operation would have no effect. */
#define LS_E_NOOP							0x0D

/*! \brief Memory-related failure. */
#define LS_E_MEMORY							0x0E

/*! \brief Operation is an unnecessary repetition. */
#define LS_E_ALREADY						0x0F

/*! \brief IO error related to closing. */
#define LS_E_IO_CLOSE						0x10

/*! \brief Target IO stream invalid. */
#define LS_E_IO_TARGET						0x11

/*! \brief IO error related to writing. */
#define LS_E_IO_WRITE						0x12

/*! \brief IO error related to flushing. */
#define LS_E_IO_FLUSH						0x13

/*! \brief Index out of bounds. */
#define LS_E_BOUNDS							0x14

/*! \brief Object or item not found. */
#define LS_E_NOT_FOUND						0x15

/*! \brief Object seems to be in an inoperable state. */
#define LS_E_INOPERABLE						0x16

/*! \brief Object seems to be out of sync. */
#define LS_E_OUT_OF_SYNC					0x17

/*! \brief Initialization failure. */
#define LS_E_INITIALIZATION					0x18

/*! \brief Data did not match what was expected. */
#define LS_E_DATA_MISMATCH					0x19

/*! \brief Algorithm error. */
#define LS_E_ALGORITHM						0x1A

/*! \brief Counter depleted. */
#define LS_E_DEPLETED						0x1B




typedef unsigned int ls_result_t;




#ifdef __cplusplus
extern "C" {
#endif

	/*!
	 * \brief ls_result_string
	 * \param result
	 * \return
	 */
	LSAPI const char* ls_result_string(const ls_result_t result);

#ifdef __cplusplus
}
#endif




#endif
