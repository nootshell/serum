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


#include "./result.h"




FILEID("");




const char *const
ls_result_string(const ls_result_t result) {
	/* Chose to do a switch over a string table: takes in less space and is less error-prone. */
	switch (result) {
		case LS_E_SUCCESS:
			return "Successful.";

		case LS_E_FAILURE:
			return "Failure.";

		case LS_E_NULL:
			return "Null-pointer encountered.";

		case LS_E_INVALID:
			return "Parameter or state/data invalid.";

		case LS_E_UNSUPPORTED:
			return "Operation not supported (not yet implemented).";

		case LS_E_MAGIC:
			return "Magic value found (or not) where inappropriate.";

		case LS_E_TIMEOUT:
			return "Operation timed out.";

		case LS_E_ABANDONED:
			return "Mutex/operation was abandoned.";

		case LS_E_SIZE:
			return "Size parameter/data invalid.";

		case LS_E_UNINITIALIZED:
			return "Object is in an uninitialized state.";

		case LS_E_CONVERSION:
			return "Conversion failure.";

		case LS_E_LOCK:
			return "Mutex-related failure.";

		case LS_E_STATE:
			return "Object is in an invalid state.";

		case LS_E_NOOP:
			return "The operation would have no effect.";

		case LS_E_MEMORY:
			return "Memory-related failure (out of memory).";

		case LS_E_ALREADY:
			return "Operation is an unnecessary repetition.";

		case LS_E_IO_CLOSE:
			return "IO error related to closing.";

		case LS_E_IO_TARGET:
			return "Target IO stream invalid.";

		case LS_E_IO_WRITE:
			return "IO error related to writing.";

		case LS_E_IO_FLUSH:
			return "IO error related to flushing.";

		case LS_E_BOUNDS:
			return "Index out of bounds.";

		case LS_E_NOT_FOUND:
			return "Object or item not found.";

		case LS_E_INOPERABLE:
			return "Object seems to be in an inoperable state.";

		case LS_E_OUT_OF_SYNC:
			return "Object seems to be out of sync.";

		case LS_E_INITIALIZATION:
			return "Initialization failure.";

		case LS_E_DATA_MISMATCH:
			return "Data did not match what was expected.";

		default:
			break;
	}

	return "Unknown.";
}
