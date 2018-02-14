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


#include "./md5min.h"

#include <string.h>




#define MD5R10(a, b, c, d, x, t, s)			(a) = (LS_ROTL32(((a) + (((d) ^ ((b) & ((c) ^ (d)))) + (x) + (t))), (s)) + (b))
#define MD5R20(a, b, c, d, x, t, s)			(a) = (LS_ROTL32(((a) + (((c) ^ ((d) & ((b) ^ (c)))) + (x) + (t))), (s)) + (b))
#define MD5R31(a, b, c, d, x, t, s)			(a) = (LS_ROTL32(((a) + ((((b) ^ (c)) ^ (d)) + (x) + (t))), (s)) + (b))
#define MD5R32(a, b, c, d, x, t, s)			(a) = (LS_ROTL32(((a) + (((b) ^ ((c) ^ (d))) + (x) + (t))), (s)) + (b))
#define MD5R40(a, b, c, d, x, t, s)			(a) = (LS_ROTL32(((a) + (((c) ^ ((b) | ~(d))) + (x) + (t))), (s)) + (b))




FILEID("Minimal MD5 implementation.");




ls_result_t
ls_md5_init(ls_md5min_data_t *const data) {
	if (data == NULL) {
		return LS_E_NULL;
	}

	data->state_A = 0x67452301U;
	data->state_B = 0xEFCDAB89U;
	data->state_C = 0x98BADCFEU;
	data->state_D = 0x10325476U;
	return LS_E_SUCCESS;
}




ls_result_t
ls_md5_transform(ls_md5min_data_t *const restrict data, const uint32_t *const restrict block) {
	if (data == NULL || block == NULL) {
		return LS_E_NULL;
	}

	register uint32_t a, b, c, d;
	uint32_t a_old, b_old, c_old, d_old;

	const uint32_t *block32;
#if (!LS_LITTLE_ENDIAN)
	block32 = block;
#else
	uint32_t le_block[16];
	for (a = (sizeof(le_block) / sizeof(*le_block)); a--;) {
		le_block[a] = LS_ENSURE_LITTLE32(block[a]);
	}
	block32 = (const uint32_t *const)le_block;
#endif

	a_old = (
		a = data->state_A
	);
	b_old = (
		b = data->state_B
	);
	c_old = (
		c = data->state_C
	);
	d_old = (
		d = data->state_D
	);

	MD5R10(a, b, c, d, block32[ 0], 0xD76AA478U,  7);
	MD5R10(d, a, b, c, block32[ 1], 0xE8C7B756U, 12);
	MD5R10(c, d, a, b, block32[ 2], 0x242070DBU, 17);
	MD5R10(b, c, d, a, block32[ 3], 0xC1BDCEEEU, 22);
	MD5R10(a, b, c, d, block32[ 4], 0xF57C0FAFU,  7);
	MD5R10(d, a, b, c, block32[ 5], 0x4787C62AU, 12);
	MD5R10(c, d, a, b, block32[ 6], 0xA8304613U, 17);
	MD5R10(b, c, d, a, block32[ 7], 0xFD469501U, 22);
	MD5R10(a, b, c, d, block32[ 8], 0x698098D8U,  7);
	MD5R10(d, a, b, c, block32[ 9], 0x8B44F7AFU, 12);
	MD5R10(c, d, a, b, block32[10], 0xFFFF5BB1U, 17);
	MD5R10(b, c, d, a, block32[11], 0x895CD7BEU, 22);
	MD5R10(a, b, c, d, block32[12], 0x6B901122U,  7);
	MD5R10(d, a, b, c, block32[13], 0xFD987193U, 12);
	MD5R10(c, d, a, b, block32[14], 0xA679438EU, 17);
	MD5R10(b, c, d, a, block32[15], 0x49B40821U, 22);

	MD5R20(a, b, c, d, block32[ 1], 0xF61E2562U,  5);
	MD5R20(d, a, b, c, block32[ 6], 0xC040B340U,  9);
	MD5R20(c, d, a, b, block32[11], 0x265E5A51U, 14);
	MD5R20(b, c, d, a, block32[ 0], 0xE9B6C7AAU, 20);
	MD5R20(a, b, c, d, block32[ 5], 0xD62F105DU,  5);
	MD5R20(d, a, b, c, block32[10], 0x02441453U,  9);
	MD5R20(c, d, a, b, block32[15], 0xD8A1E681U, 14);
	MD5R20(b, c, d, a, block32[ 4], 0xE7D3FBC8U, 20);
	MD5R20(a, b, c, d, block32[ 9], 0x21E1CDE6U,  5);
	MD5R20(d, a, b, c, block32[14], 0xC33707D6U,  9);
	MD5R20(c, d, a, b, block32[ 3], 0xF4D50D87U, 14);
	MD5R20(b, c, d, a, block32[ 8], 0x455A14EDU, 20);
	MD5R20(a, b, c, d, block32[13], 0xA9E3E905U,  5);
	MD5R20(d, a, b, c, block32[ 2], 0xFCEFA3F8U,  9);
	MD5R20(c, d, a, b, block32[ 7], 0x676F02D9U, 14);
	MD5R20(b, c, d, a, block32[12], 0x8D2A4C8AU, 20);

	MD5R31(a, b, c, d, block32[ 5], 0xFFFA3942U,  4);
	MD5R32(d, a, b, c, block32[ 8], 0x8771F681U, 11);
	MD5R31(c, d, a, b, block32[11], 0x6D9D6122U, 16);
	MD5R32(b, c, d, a, block32[14], 0xFDE5380CU, 23);
	MD5R31(a, b, c, d, block32[ 1], 0xA4BEEA44U,  4);
	MD5R32(d, a, b, c, block32[ 4], 0x4BDECFA9U, 11);
	MD5R31(c, d, a, b, block32[ 7], 0xF6BB4B60U, 16);
	MD5R32(b, c, d, a, block32[10], 0xBEBFBC70U, 23);
	MD5R31(a, b, c, d, block32[13], 0x289B7EC6U,  4);
	MD5R32(d, a, b, c, block32[ 0], 0xEAA127FAU, 11);
	MD5R31(c, d, a, b, block32[ 3], 0xD4EF3085U, 16);
	MD5R32(b, c, d, a, block32[ 6], 0x04881D05U, 23);
	MD5R31(a, b, c, d, block32[ 9], 0xD9D4D039U,  4);
	MD5R32(d, a, b, c, block32[12], 0xE6DB99E5U, 11);
	MD5R31(c, d, a, b, block32[15], 0x1FA27CF8U, 16);
	MD5R32(b, c, d, a, block32[ 2], 0xC4AC5665U, 23);

	MD5R40(a, b, c, d, block32[ 0], 0xF4292244U,  6);
	MD5R40(d, a, b, c, block32[ 7], 0x432AFF97U, 10);
	MD5R40(c, d, a, b, block32[14], 0xAB9423A7U, 15);
	MD5R40(b, c, d, a, block32[ 5], 0xFC93A039U, 21);
	MD5R40(a, b, c, d, block32[12], 0x655B59C3U,  6);
	MD5R40(d, a, b, c, block32[ 3], 0x8F0CCC92U, 10);
	MD5R40(c, d, a, b, block32[10], 0xFFEFF47DU, 15);
	MD5R40(b, c, d, a, block32[ 1], 0x85845DD1U, 21);
	MD5R40(a, b, c, d, block32[ 8], 0x6FA87E4FU,  6);
	MD5R40(d, a, b, c, block32[15], 0xFE2CE6E0U, 10);
	MD5R40(c, d, a, b, block32[ 6], 0xA3014314U, 15);
	MD5R40(b, c, d, a, block32[13], 0x4E0811A1U, 21);
	MD5R40(a, b, c, d, block32[ 4], 0xF7537E82U,  6);
	MD5R40(d, a, b, c, block32[11], 0xBD3AF235U, 10);
	MD5R40(c, d, a, b, block32[ 2], 0x2AD7D2BBU, 15);
	MD5R40(b, c, d, a, block32[ 9], 0xEB86D391U, 21);

	data->state_A = (a_old + a);
	data->state_B = (b_old + b);
	data->state_C = (c_old + c);
	data->state_D = (d_old + d);

	return LS_E_SUCCESS;
}


ls_result_t
ls_md5_finish(ls_md5min_data_t *const restrict data, const uint8_t *const restrict input, size_t size, const size_t bits, ls_md5_digest_t digest) {
	if (data == NULL || (size > 0 && input == NULL) || digest == NULL) {
		return LS_E_NULL;
	}

	if (size > LS_MD5_BLOCK_SIZE) {
		return LS_E_SIZE;
	}

	// Populate the buffer with remaining input (if any), and the starting
	// bit of the padding.
	uint8_t buffer[LS_MD5_BLOCK_SIZE];
	if (size > 0) {
		memcpy(buffer, input, size);
	}
	buffer[size++] = 0x80;

	// Check if the buffer can hold the input and the final padding and if
	// not so, process what we have so far (split the finalization into two
	// transformations).
	size_t diff = (LS_MD5_BLOCK_SIZE - size);
	if (diff < 8) {
		if (size < LS_MD5_BLOCK_SIZE) {
			memset(&buffer[size], 0, diff);
		}
		if (ls_md5_transform(data, (const uint32_t *const)buffer) != LS_E_SUCCESS) {
			return LS_E_FAILURE;
		}
		size = 0;
		diff = LS_MD5_BLOCK_SIZE;
	}

	// Clear everything between the input and the final padding.
	memset(&buffer[size], 0, (diff - 8));

	// Append the number of bits in the message to the end of the pad and
	// perform the final transform.
	*((uint64_t*)(buffer + (LS_MD5_BLOCK_SIZE - 8))) = LS_ENSURE_LITTLE64(bits);
	if (ls_md5_transform(data, (const uint32_t *const)buffer) != LS_E_SUCCESS) {
		return LS_E_FAILURE;
	}

	// Output the digest.
	uint32_t *const dout32 = (uint32_t *const)digest;
	dout32[0] = LS_ENSURE_LITTLE32(data->state_A);
	dout32[1] = LS_ENSURE_LITTLE32(data->state_B);
	dout32[2] = LS_ENSURE_LITTLE32(data->state_C);
	dout32[3] = LS_ENSURE_LITTLE32(data->state_D);

	// Clear the context.
	data->state_A = data->state_B = data->state_C = data->state_D = 0;

	return LS_E_SUCCESS;
}
