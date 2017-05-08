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

#define FILE_PATH							"crypto/prng/device.c"

#include "./device.h"
#include "../../core/math.h"
#include "../../core/memory.h"
#include <string.h>


ID("filesystem-based PRNG");


#if (defined(LS_DEVICE_BUFFER_BLOCK))
#	if (LS_DEVICE_BUFFER_BLOCK < 0)
#		undef LS_DEVICE_BUFFER_BLOCK
#	endif
#endif

#if (!defined(LS_DEVICE_BUFFER_BLOCK))
#	define LS_DEVICE_BUFFER_BLOCK			16
#endif


#if (LS_WINDOWS)
#	include <io.h>
#	define access							_access
#	define F_OK								00
#	define W_OK								02
#	define R_OK								04
#	define RW_OK							(R_OK | W_OK)
#else
#	include <unistd.h>
#endif


ls_result_t
ls_device_init(ls_device_t *const LS_RESTRICT device, const char *const LS_RESTRICT file, const size_t buffer_size) {
	LS_RESULT_CHECK_NULL(device, 1);
	LS_RESULT_CHECK_NULL(file, 2);

	// First poke the file.
	if (access(file, (F_OK | R_OK)) != 0) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_ACCESS);
	}

	// Set the buffer size.
	device->buffer_size = LS_MATH_ROUND_BLOCK_EXCL(LS_DEVICE_BUFFER_BLOCK, buffer_size);
	if (device->buffer_size < LS_DEVICE_BUFFER_BLOCK) {
		ls_log(LS_LOG_WARNING, "buffer size below " MACRO_STRINGIFY(LS_DEVICE_BUFFER_BLOCK) ", corrected");
		device->buffer_size = LS_DEVICE_BUFFER_BLOCK;
	}

	uint16_t code = LS_RESULT_CODE_SUCCESS;

	// Allocate memory for the buffer.
	device->buffer = malloc(device->buffer_size);
	if (!device->buffer) {
		code = LS_RESULT_CODE_ALLOCATION;
		goto __cleanup;
	}

	// Lock the buffer memory.
	if (!LS_MEMLOCK(device->buffer, device->buffer_size)) {
		code = LS_RESULT_CODE_LOCK;
		goto __cleanup;
	}

	// Finally open the file.
	if (!(device->fp = fopen(file, "r"))) {
		code = LS_RESULT_CODE_DESCRIPTOR;
		goto __cleanup;
	}

	return LS_RESULT_SUCCESS;

__cleanup:
	if (device->buffer) {
		if (code == LS_RESULT_CODE_DESCRIPTOR) {
			LS_MEMUNLOCK(device->buffer, device->buffer_size);
		}

		free(device->buffer);
		device->buffer = NULL;
	}
	device->buffer_size = 0;

	return LS_RESULT_ERROR(code);
}


ls_result_t
ls_device_clear(ls_device_t *const device) {
	LS_RESULT_CHECK_NULL(device, 1);

	if (device->fp) {
		if (fclose(device->fp) != 0) {
			return LS_RESULT_ERROR(LS_RESULT_CODE_DESCRIPTOR);
		}
		device->fp = NULL;
	}

	if (device->buffer) {
		if (device->buffer_size) {
			memset(device->buffer, 0, device->buffer_size);
			LS_MEMUNLOCK(device->buffer, device->buffer_size);
		}
		free(device->buffer);
		device->buffer = NULL;
	}
	device->buffer_size = 0;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_device_sys(ls_device_t *const device, const size_t buffer_size, const ls_prng_device_type_t type) {
	LS_RESULT_CHECK_NULL(device, 1);

#if (LS_LINUX || LS_MAC)
#	define TRY_DEVICE(path) { if (ls_device_init(device, (path), buffer_size).success) { return LS_RESULT_SUCCESS; } }

	if (HAS_FLAG(type, DEV_URANDOM)) {
		TRY_DEVICE("/dev/urandom");
	}

	if (HAS_FLAG(type, DEV_HARDWARE)) {
		TRY_DEVICE("/dev/hwrng");
	}

	TRY_DEVICE("/dev/random");

	if (HAS_FLAG(type, DEV_URANDOM_FALLBACK)) {
		TRY_DEVICE("/dev/urandom");
	}

	return LS_RESULT_ERROR(LS_RESULT_CODE_NOT_FOUND);

#	undef TRY_DEVICE
#else
	return LS_RESULT_ERROR(LS_RESULT_CODE_UNSUPPORTED);
#endif
}


ls_result_t
ls_device_generate(const ls_device_t *const LS_RESTRICT device, void *const LS_RESTRICT out, size_t size) {
	LS_RESULT_CHECK_NULL(device, 1);
	LS_RESULT_CHECK_NULL(out, 2);
	LS_RESULT_CHECK_NULL(device->buffer, 3);
	LS_RESULT_CHECK(!device->fp, LS_RESULT_CODE_DESCRIPTOR, 1);
	LS_RESULT_CHECK_SIZE(size, 1);

	size_t
		read,
		read_total = 0,
		rsz = sizeof(int),
		rsz_num = (device->buffer_size / rsz);

	for (;;) {
		if (size < rsz) {
			rsz = 1;
			rsz_num = size;
		} else if (size < device->buffer_size) {
			rsz_num = (size / rsz);
		}

		if ((read = (fread(device->buffer, rsz, rsz_num, device->fp) * rsz)) > 0) {
			memcpy((((char*)out) + read_total), device->buffer, read);

			size -= read;
			read_total += read;

			if (!size) {
				break;
			}
		} else {
			return LS_RESULT_ERROR(LS_RESULT_CODE_READ);
		}
	}

	return (size ? LS_RESULT_ERROR(LS_RESULT_CODE_EARLY_EXIT) : LS_RESULT_SUCCESS);
}
