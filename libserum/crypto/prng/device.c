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

#include <string.h>
#include <sys/mman.h> // Don't give a jack *&^# about Windows for now
#include <unistd.h>
#include "./device.h"
#include "../../core/math.h"


ID("PRNG using a device as source");


ls_result_t
ls_prng_device_init(ls_prng_device_t *device, const char *file, size_t buffer_size) {
	if (!device) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}

	if (!file) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 2);
	}

	if (access(file, (F_OK|R_OK)) != 0) {
        return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_ACCESS, 1);
	}

    if (!(device->fp = fopen(file, "r"))) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_DESCRIPTOR);
    }

	device->buffer = malloc(
		device->buffer_size = LS_MATH_ROUND_BLOCK_INCL(64, buffer_size)
	);
	if (!device->buffer) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_ALLOCATION);
	}

	if (mlock(device->buffer, device->buffer_size) != 0) {
		free(device->buffer);
		device->buffer = NULL;
		return LS_RESULT_ERROR(LS_RESULT_CODE_LOCK);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_prng_device_clear(ls_prng_device_t *device) {
	if (!device) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}

	if (device->buffer && device->buffer_size) {
		memset(device->buffer, 0, device->buffer_size);
		free(device->buffer);
	}

	if (fclose(device->fp) != 0) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_DESCRIPTOR);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_prng_device_generate(ls_prng_device_t *device, void *out, size_t size) {
	if (!device) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}

	if (!device->fp) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 2);
	}

	if (!device->buffer) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 3);
	}

    if (!out) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 4);
    }

    if (!size) {
        return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_SIZE, 1);
    }

    size_t read_total = 0, read;
    while (((read = fread(device->buffer, 1, device->buffer_size, device->fp)) > 0) && (read_total < size)) {
		if ((read_total + read) > size) {
            read = (size - read_total);
		}
		memcpy((out + read_total), device->buffer, read);
		read_total += read;
    }

    return ((read_total == size) ? LS_RESULT_SUCCESS : LS_RESULT_ERROR(LS_RESULT_CODE_EARLY_EXIT));
}


ls_result_t
ls_prng_device_sys(ls_prng_device_t *device, size_t buffer_size, ls_prng_device_type_t type) {
	if (!device) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}

#if (LS_LINUX || LS_MAC)
#	define TRY_DEVICE(path) { if (ls_prng_device_init(device, (path), buffer_size).success) { return LS_RESULT_SUCCESS; } }

	if (HAS_FLAG(type, DEV_FORCE_UNLIMITED)) {
		TRY_DEVICE("/dev/urandom");
	}

	if (HAS_FLAG(type, DEV_HARDWARE)) {
		TRY_DEVICE("/dev/hwrng");
	}

	TRY_DEVICE("/dev/random");

	if (HAS_FLAG(type, DEV_URANDOM)) {
		TRY_DEVICE("/dev/urandom");
	}

#	undef TRY_DEVICE
#endif

	return LS_RESULT_ERROR(LS_RESULT_CODE_UNSUPPORTED);
}
