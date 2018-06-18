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


#include "./ripemd160.h"

#include "../hash_utils.h"




FILEID("RIPEMD-160 implementation.");




ls_result_t
ls_ripemd160_init(ls_ripemd160_t *const context) {
	if (context == NULL) {
		return_e(LS_E_NULL);
	}

	context->length = 0;
	context->state_A = 0x67452301;
	context->state_B = 0xEFCDAB89;
	context->state_C = 0x98BADCFE;
	context->state_D = 0x10325476;
	context->state_E = 0xC3D2E1F0;

	return LS_E_SUCCESS;
}




#define RIPEMD_XX(x, y, z)					((x) ^ (y) ^ (z))
#define RIPEMD_ONX(x, y, z)					(((x) | ~(y)) ^ (z))
#define RIPEMD_XON(x, y, z)					((x) ^ ((y) | ~(z)))
#define RIPEMD_AONA(x, y, z)				(((x) & (y)) | (~(x) & (z)))
#define RIPEMD_AOAN(x, y, z)				(((x) & (z)) | ((y) & ~(z)))

#define RIPEMD_B1_R1(a, b, c, d, e, x, s)	{ (a) += RIPEMD_XX((b), (c), (d)) + (x);               (a) = LS_ROTL32((a), (s)) + (e); (c) = LS_ROTL32((c), 10); }
#define RIPEMD_B1_R2(a, b, c, d, e, x, s)	{ (a) += RIPEMD_AONA((b), (c), (d)) + (x) + 0x5A827999U; (a) = LS_ROTL32((a), (s)) + (e); (c) = LS_ROTL32((c), 10); }
#define RIPEMD_B1_R3(a, b, c, d, e, x, s)	{ (a) += RIPEMD_ONX((b), (c), (d)) + (x) + 0x6ED9EBA1U; (a) = LS_ROTL32((a), (s)) + (e); (c) = LS_ROTL32((c), 10); }
#define RIPEMD_B1_R4(a, b, c, d, e, x, s)	{ (a) += RIPEMD_AOAN((b), (c), (d)) + (x) + 0x8F1BBCDCU; (a) = LS_ROTL32((a), (s)) + (e); (c) = LS_ROTL32((c), 10); }
#define RIPEMD_B1_R5(a, b, c, d, e, x, s)	{ (a) += RIPEMD_XON((b), (c), (d)) + (x) + 0xA953FD4EU; (a) = LS_ROTL32((a), (s)) + (e); (c) = LS_ROTL32((c), 10); }

#define RIPEMD_B2_R1(a, b, c, d, e, x, s)	{ (a) += RIPEMD_XON((b), (c), (d)) + (x) + 0x50A28BE6U; (a) = LS_ROTL32((a), (s)) + (e); (c) = LS_ROTL32((c), 10); }
#define RIPEMD_B2_R2(a, b, c, d, e, x, s)	{ (a) += RIPEMD_AOAN((b), (c), (d)) + (x) + 0x5C4DD124U; (a) = LS_ROTL32((a), (s)) + (e); (c) = LS_ROTL32((c), 10); }
#define RIPEMD_B2_R3(a, b, c, d, e, x, s)	{ (a) += RIPEMD_ONX((b), (c), (d)) + (x) + 0x6D703EF3U; (a) = LS_ROTL32((a), (s)) + (e); (c) = LS_ROTL32((c), 10); }
#define RIPEMD_B2_R4(a, b, c, d, e, x, s)	{ (a) += RIPEMD_AONA((b), (c), (d)) + (x) + 0x7A6D76E9U; (a) = LS_ROTL32((a), (s)) + (e); (c) = LS_ROTL32((c), 10); }
#define RIPEMD_B2_R5(a, b, c, d, e, x, s)	{ (a) += RIPEMD_XX((b), (c), (d)) + (x);               (a) = LS_ROTL32((a), (s)) + (e); (c) = LS_ROTL32((c), 10); }


ls_result_t
ls_ripemd160_update(ls_ripemd160_t *const context, const ls_ripemd160_block_t block) {
	if (context == NULL || block == NULL) {
		return_e(LS_E_NULL);
	}


	const uint32_t *const X = (const uint32_t *const)block;

	uint32_t store[5];
	register uint32_t reg_A = context->state_A;
	register uint32_t reg_B = context->state_B;
	register uint32_t reg_C = context->state_C;
	register uint32_t reg_D = context->state_D;
	register uint32_t reg_E = context->state_E;


	RIPEMD_B1_R1(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 0], 11);
	RIPEMD_B1_R1(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 1], 14);
	RIPEMD_B1_R1(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 2], 15);
	RIPEMD_B1_R1(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 3], 12);
	RIPEMD_B1_R1(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 4],  5);
	RIPEMD_B1_R1(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 5],  8);
	RIPEMD_B1_R1(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 6],  7);
	RIPEMD_B1_R1(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 7],  9);
	RIPEMD_B1_R1(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 8], 11);
	RIPEMD_B1_R1(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 9], 13);
	RIPEMD_B1_R1(reg_A, reg_B, reg_C, reg_D, reg_E, X[10], 14);
	RIPEMD_B1_R1(reg_E, reg_A, reg_B, reg_C, reg_D, X[11], 15);
	RIPEMD_B1_R1(reg_D, reg_E, reg_A, reg_B, reg_C, X[12],  6);
	RIPEMD_B1_R1(reg_C, reg_D, reg_E, reg_A, reg_B, X[13],  7);
	RIPEMD_B1_R1(reg_B, reg_C, reg_D, reg_E, reg_A, X[14],  9);
	RIPEMD_B1_R1(reg_A, reg_B, reg_C, reg_D, reg_E, X[15],  8);

	RIPEMD_B1_R2(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 7],  7);
	RIPEMD_B1_R2(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 4],  6);
	RIPEMD_B1_R2(reg_C, reg_D, reg_E, reg_A, reg_B, X[13],  8);
	RIPEMD_B1_R2(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 1], 13);
	RIPEMD_B1_R2(reg_A, reg_B, reg_C, reg_D, reg_E, X[10], 11);
	RIPEMD_B1_R2(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 6],  9);
	RIPEMD_B1_R2(reg_D, reg_E, reg_A, reg_B, reg_C, X[15],  7);
	RIPEMD_B1_R2(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 3], 15);
	RIPEMD_B1_R2(reg_B, reg_C, reg_D, reg_E, reg_A, X[12],  7);
	RIPEMD_B1_R2(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 0], 12);
	RIPEMD_B1_R2(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 9], 15);
	RIPEMD_B1_R2(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 5],  9);
	RIPEMD_B1_R2(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 2], 11);
	RIPEMD_B1_R2(reg_B, reg_C, reg_D, reg_E, reg_A, X[14],  7);
	RIPEMD_B1_R2(reg_A, reg_B, reg_C, reg_D, reg_E, X[11], 13);
	RIPEMD_B1_R2(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 8], 12);

	RIPEMD_B1_R3(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 3], 11);
	RIPEMD_B1_R3(reg_C, reg_D, reg_E, reg_A, reg_B, X[10], 13);
	RIPEMD_B1_R3(reg_B, reg_C, reg_D, reg_E, reg_A, X[14],  6);
	RIPEMD_B1_R3(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 4],  7);
	RIPEMD_B1_R3(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 9], 14);
	RIPEMD_B1_R3(reg_D, reg_E, reg_A, reg_B, reg_C, X[15],  9);
	RIPEMD_B1_R3(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 8], 13);
	RIPEMD_B1_R3(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 1], 15);
	RIPEMD_B1_R3(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 2], 14);
	RIPEMD_B1_R3(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 7],  8);
	RIPEMD_B1_R3(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 0], 13);
	RIPEMD_B1_R3(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 6],  6);
	RIPEMD_B1_R3(reg_B, reg_C, reg_D, reg_E, reg_A, X[13],  5);
	RIPEMD_B1_R3(reg_A, reg_B, reg_C, reg_D, reg_E, X[11], 12);
	RIPEMD_B1_R3(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 5],  7);
	RIPEMD_B1_R3(reg_D, reg_E, reg_A, reg_B, reg_C, X[12],  5);

	RIPEMD_B1_R4(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 1], 11);
	RIPEMD_B1_R4(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 9], 12);
	RIPEMD_B1_R4(reg_A, reg_B, reg_C, reg_D, reg_E, X[11], 14);
	RIPEMD_B1_R4(reg_E, reg_A, reg_B, reg_C, reg_D, X[10], 15);
	RIPEMD_B1_R4(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 0], 14);
	RIPEMD_B1_R4(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 8], 15);
	RIPEMD_B1_R4(reg_B, reg_C, reg_D, reg_E, reg_A, X[12],  9);
	RIPEMD_B1_R4(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 4],  8);
	RIPEMD_B1_R4(reg_E, reg_A, reg_B, reg_C, reg_D, X[13],  9);
	RIPEMD_B1_R4(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 3], 14);
	RIPEMD_B1_R4(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 7],  5);
	RIPEMD_B1_R4(reg_B, reg_C, reg_D, reg_E, reg_A, X[15],  6);
	RIPEMD_B1_R4(reg_A, reg_B, reg_C, reg_D, reg_E, X[14],  8);
	RIPEMD_B1_R4(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 5],  6);
	RIPEMD_B1_R4(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 6],  5);
	RIPEMD_B1_R4(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 2], 12);

	RIPEMD_B1_R5(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 4],  9);
	RIPEMD_B1_R5(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 0], 15);
	RIPEMD_B1_R5(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 5],  5);
	RIPEMD_B1_R5(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 9], 11);
	RIPEMD_B1_R5(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 7],  6);
	RIPEMD_B1_R5(reg_B, reg_C, reg_D, reg_E, reg_A, X[12],  8);
	RIPEMD_B1_R5(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 2], 13);
	RIPEMD_B1_R5(reg_E, reg_A, reg_B, reg_C, reg_D, X[10], 12);
	RIPEMD_B1_R5(reg_D, reg_E, reg_A, reg_B, reg_C, X[14],  5);
	RIPEMD_B1_R5(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 1], 12);
	RIPEMD_B1_R5(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 3], 13);
	RIPEMD_B1_R5(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 8], 14);
	RIPEMD_B1_R5(reg_E, reg_A, reg_B, reg_C, reg_D, X[11], 11);
	RIPEMD_B1_R5(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 6],  8);
	RIPEMD_B1_R5(reg_C, reg_D, reg_E, reg_A, reg_B, X[15],  5);
	RIPEMD_B1_R5(reg_B, reg_C, reg_D, reg_E, reg_A, X[13],  6);

	store[0] = reg_A;
	store[1] = reg_B;
	store[2] = reg_C;
	store[3] = reg_D;
	store[4] = reg_E;


	reg_A = context->state_A;
	reg_B = context->state_B;
	reg_C = context->state_C;
	reg_D = context->state_D;
	reg_E = context->state_E;

	RIPEMD_B2_R1(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 5],  8);
	RIPEMD_B2_R1(reg_E, reg_A, reg_B, reg_C, reg_D, X[14],  9);
	RIPEMD_B2_R1(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 7],  9);
	RIPEMD_B2_R1(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 0], 11);
	RIPEMD_B2_R1(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 9], 13);
	RIPEMD_B2_R1(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 2], 15);
	RIPEMD_B2_R1(reg_E, reg_A, reg_B, reg_C, reg_D, X[11], 15);
	RIPEMD_B2_R1(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 4],  5);
	RIPEMD_B2_R1(reg_C, reg_D, reg_E, reg_A, reg_B, X[13],  7);
	RIPEMD_B2_R1(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 6],  7);
	RIPEMD_B2_R1(reg_A, reg_B, reg_C, reg_D, reg_E, X[15],  8);
	RIPEMD_B2_R1(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 8], 11);
	RIPEMD_B2_R1(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 1], 14);
	RIPEMD_B2_R1(reg_C, reg_D, reg_E, reg_A, reg_B, X[10], 14);
	RIPEMD_B2_R1(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 3], 12);
	RIPEMD_B2_R1(reg_A, reg_B, reg_C, reg_D, reg_E, X[12],  6);

	RIPEMD_B2_R2(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 6],  9);
	RIPEMD_B2_R2(reg_D, reg_E, reg_A, reg_B, reg_C, X[11], 13);
	RIPEMD_B2_R2(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 3], 15);
	RIPEMD_B2_R2(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 7],  7);
	RIPEMD_B2_R2(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 0], 12);
	RIPEMD_B2_R2(reg_E, reg_A, reg_B, reg_C, reg_D, X[13],  8);
	RIPEMD_B2_R2(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 5],  9);
	RIPEMD_B2_R2(reg_C, reg_D, reg_E, reg_A, reg_B, X[10], 11);
	RIPEMD_B2_R2(reg_B, reg_C, reg_D, reg_E, reg_A, X[14],  7);
	RIPEMD_B2_R2(reg_A, reg_B, reg_C, reg_D, reg_E, X[15],  7);
	RIPEMD_B2_R2(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 8], 12);
	RIPEMD_B2_R2(reg_D, reg_E, reg_A, reg_B, reg_C, X[12],  7);
	RIPEMD_B2_R2(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 4],  6);
	RIPEMD_B2_R2(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 9], 15);
	RIPEMD_B2_R2(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 1], 13);
	RIPEMD_B2_R2(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 2], 11);

	RIPEMD_B2_R3(reg_D, reg_E, reg_A, reg_B, reg_C, X[15],  9);
	RIPEMD_B2_R3(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 5],  7);
	RIPEMD_B2_R3(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 1], 15);
	RIPEMD_B2_R3(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 3], 11);
	RIPEMD_B2_R3(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 7],  8);
	RIPEMD_B2_R3(reg_D, reg_E, reg_A, reg_B, reg_C, X[14],  6);
	RIPEMD_B2_R3(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 6],  6);
	RIPEMD_B2_R3(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 9], 14);
	RIPEMD_B2_R3(reg_A, reg_B, reg_C, reg_D, reg_E, X[11], 12);
	RIPEMD_B2_R3(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 8], 13);
	RIPEMD_B2_R3(reg_D, reg_E, reg_A, reg_B, reg_C, X[12],  5);
	RIPEMD_B2_R3(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 2], 14);
	RIPEMD_B2_R3(reg_B, reg_C, reg_D, reg_E, reg_A, X[10], 13);
	RIPEMD_B2_R3(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 0], 13);
	RIPEMD_B2_R3(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 4],  7);
	RIPEMD_B2_R3(reg_D, reg_E, reg_A, reg_B, reg_C, X[13],  5);

	RIPEMD_B2_R4(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 8], 15);
	RIPEMD_B2_R4(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 6],  5);
	RIPEMD_B2_R4(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 4],  8);
	RIPEMD_B2_R4(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 1], 11);
	RIPEMD_B2_R4(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 3], 14);
	RIPEMD_B2_R4(reg_C, reg_D, reg_E, reg_A, reg_B, X[11], 14);
	RIPEMD_B2_R4(reg_B, reg_C, reg_D, reg_E, reg_A, X[15],  6);
	RIPEMD_B2_R4(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 0], 14);
	RIPEMD_B2_R4(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 5],  6);
	RIPEMD_B2_R4(reg_D, reg_E, reg_A, reg_B, reg_C, X[12],  9);
	RIPEMD_B2_R4(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 2], 12);
	RIPEMD_B2_R4(reg_B, reg_C, reg_D, reg_E, reg_A, X[13],  9);
	RIPEMD_B2_R4(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 9], 12);
	RIPEMD_B2_R4(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 7],  5);
	RIPEMD_B2_R4(reg_D, reg_E, reg_A, reg_B, reg_C, X[10], 15);
	RIPEMD_B2_R4(reg_C, reg_D, reg_E, reg_A, reg_B, X[14],  8);

	RIPEMD_B2_R5(reg_B, reg_C, reg_D, reg_E, reg_A, X[12],  8);
	RIPEMD_B2_R5(reg_A, reg_B, reg_C, reg_D, reg_E, X[15],  5);
	RIPEMD_B2_R5(reg_E, reg_A, reg_B, reg_C, reg_D, X[10], 12);
	RIPEMD_B2_R5(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 4],  9);
	RIPEMD_B2_R5(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 1], 12);
	RIPEMD_B2_R5(reg_B, reg_C, reg_D, reg_E, reg_A, X[ 5],  5);
	RIPEMD_B2_R5(reg_A, reg_B, reg_C, reg_D, reg_E, X[ 8], 14);
	RIPEMD_B2_R5(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 7],  6);
	RIPEMD_B2_R5(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 6],  8);
	RIPEMD_B2_R5(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 2], 13);
	RIPEMD_B2_R5(reg_B, reg_C, reg_D, reg_E, reg_A, X[13],  6);
	RIPEMD_B2_R5(reg_A, reg_B, reg_C, reg_D, reg_E, X[14],  5);
	RIPEMD_B2_R5(reg_E, reg_A, reg_B, reg_C, reg_D, X[ 0], 15);
	RIPEMD_B2_R5(reg_D, reg_E, reg_A, reg_B, reg_C, X[ 3], 13);
	RIPEMD_B2_R5(reg_C, reg_D, reg_E, reg_A, reg_B, X[ 9], 11);
	RIPEMD_B2_R5(reg_B, reg_C, reg_D, reg_E, reg_A, X[11], 11);


	context->length += LS_RIPEMD160_BLOCK_SIZE;

	reg_D += store[2] + context->state_B;
	context->state_B = context->state_C + store[3] + reg_E;
	context->state_C = context->state_D + store[4] + reg_A;
	context->state_D = context->state_E + store[0] + reg_B;
	context->state_E = context->state_A + store[1] + reg_C;
	context->state_A = reg_D;


	return LS_E_SUCCESS;
}


#undef RIPEMD_XX
#undef RIPEMD_ONX
#undef RIPEMD_XON
#undef RIPEMD_AONA
#undef RIPEMD_AOAN

#undef RIPEMD_B1_R1
#undef RIPEMD_B1_R2
#undef RIPEMD_B1_R3
#undef RIPEMD_B1_R4
#undef RIPEMD_B1_R5

#undef RIPEMD_B2_R1
#undef RIPEMD_B2_R2
#undef RIPEMD_B2_R3
#undef RIPEMD_B2_R4
#undef RIPEMD_B2_R5




ls_result_t
ls_ripemd160_finish(ls_ripemd160_t *const restrict context, const uint8_t *const restrict input, const size_t input_size, ls_ripemd160_digest_t out_digest) {
	if (context == NULL || (input_size > 0 && input == NULL) || out_digest == NULL) {
		return_e(LS_E_NULL);
	}

	if (input_size > LS_RIPEMD160_BLOCK_SIZE) {
		return_e(LS_E_SIZE);
	}

	/* Pad the message, and perform the final transformation. */
	{
		ls_ripemd160_block_t buffer;

		const ls_result_t pad_result = __ls_hash_finish_80_00_length64(
			context, (lssig_hash_update)ls_ripemd160_update,
			((context->length + input_size) * LS_BITS_BYTE),
			buffer, LS_RIPEMD160_BLOCK_SIZE,
			input, input_size
		);

		if (pad_result != LS_E_SUCCESS) {
			return pad_result;
		}
	}

	/* Output the digest. */
	uint32_t *const dout32 = (uint32_t *const)out_digest;
	dout32[0] = LS_ENSURE_LITTLE32(context->state_A);
	dout32[1] = LS_ENSURE_LITTLE32(context->state_B);
	dout32[2] = LS_ENSURE_LITTLE32(context->state_C);
	dout32[3] = LS_ENSURE_LITTLE32(context->state_D);
	dout32[4] = LS_ENSURE_LITTLE32(context->state_E);

	/* Clear the context. */
	context->length = context->state_A = context->state_B = context->state_C = context->state_D = context->state_E = 0;

	return LS_E_SUCCESS;
}
