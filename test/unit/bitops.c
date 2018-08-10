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


#include "./bitops.h"

#include <libserum/io/log.h>




#define DIRECTION_L							0x0001
#define DIRECTION_R							0x0002




int
lst_unit_bitops() {
	int result = 0;


	union tvalue {
		uint8_t u8;
		uint16_t u16;
		uint32_t u32;
		uint64_t u64;
	};

	const static struct vect {
		unsigned int rotate;
		uint16_t direction;
		uint16_t size;
		union tvalue seed;
		union tvalue target;
	} vector_list[] = {
		{ .seed = { .u8 = 0x01 }, .direction = DIRECTION_L, .rotate = 7, .target = { .u8 = 0x80 }, .size = 1 },
		{ .seed = { .u8 = 0x04 }, .direction = DIRECTION_L, .rotate = 3, .target = { .u8 = 0x20 }, .size = 1 },
		{ .seed = { .u8 = 0x40 }, .direction = DIRECTION_R, .rotate = 4, .target = { .u8 = 0x04 }, .size = 1 },
		{ .seed = { .u8 = 0x10 }, .direction = DIRECTION_R, .rotate = 5, .target = { .u8 = 0x80 }, .size = 1 }
	};


	ls_bool_t left = false;

	const struct vect *vector = NULL;

	const unsigned int i_end = (sizeof(vector_list) / sizeof(*vector_list));
	register unsigned int i;

	for (i = 0; i < i_end; ++i) {
		vector = &vector_list[i];
		left = (vector->direction == DIRECTION_L);

#define assert_vec(expr, desc, fmt, seed)	if (!(expr)) { result = 1; ls_log_writeln(NULL, LS_LOG_LEVEL_ERROR, "Unit failed: bitops/%s (" fmt ")", desc, seed); }

		switch (vector->size) {
			case 1: {
					assert_vec(((left ? __ls_rotl8 : __ls_rotr8)(vector->seed.u8, vector->rotate) == vector->target.u8), (left ? "rotl8" : "rotr8"), "0x%02X", vector->target.u8);
				}
				break;
			case 2: {
					assert_vec(((left ? __ls_rotl16 : __ls_rotr16)(vector->seed.u16, vector->rotate) == vector->target.u16), (left ? "rotl16" : "rotr16"), "0x%04X", vector->target.u16);
				}
				break;
			case 4: {
					assert_vec(((left ? __ls_rotl32 : __ls_rotr32)(vector->seed.u32, vector->rotate) == vector->target.u32), (left ? "rotl32" : "rotr32"), "0x%08X", vector->target.u32);
				}
				break;
			case 8: {
					assert_vec(((left ? __ls_rotl64 : __ls_rotr64)(vector->seed.u64, vector->rotate) == vector->target.u64), (left ? "rotl64" : "rotr64"), "0x%016X", vector->target.u64);
				}
				break;
			default:
				result = 2;
				break;
		}
	}


	return result;
}
