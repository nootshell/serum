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
**    Derived from agl/curve25519-donna, copyright notice in the comment block
**    below.
**
*/

/* Copyright 2008, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * curve25519-donna: Curve25519 elliptic curve, public key function
 *
 * http://code.google.com/p/curve25519-donna/
 *
 * Adam Langley <agl@imperialviolet.org>
 *
 * Derived from public domain C code by Daniel J. Bernstein <djb@cr.yp.to>
 *
 * More information about curve25519 can be found here
 *   http://cr.yp.to/ecdh.html
 *
 * djb's sample implementation of curve25519 is written in a special assembly
 * language called qhasm and uses the floating point registers.
 *
 * This is, almost, a clean room reimplementation from the curve25519 paper. It
 * uses many of the tricks described therein. Only the crecip function is taken
 * from the sample implementation. */


#if ((-1 & 3) == 3)			// This code only works on a two's complement system.
#	if ((-32 >> 1) == -16)	// This code only works when >> does sign-extension on negative numbers.

#define FILE_PATH							"math/curve25519.c"

#include "./curve25519.h"
#include <string.h>


ID("Curve25519 implementation");


#define INTARR_SIZE							(sizeof(int64_t) * 10)


void
static LS_ATTR_INLINE fsum(int64_t *const LS_RESTRICT output, const int64_t *const LS_RESTRICT in) {
	ls_nword_t i;
	for (i = 10; i--;) {
		output[i] += in[i];
	}
}


void
static LS_ATTR_INLINE fdifference(int64_t *const LS_RESTRICT output, const int64_t *const LS_RESTRICT in) {
	ls_nword_t i;
	for (i = 10; i--;) {
		output[i] = (in[i] - output[i]);
	}
}


void
static fproduct(int64_t *const LS_RESTRICT output, const int64_t *const LS_RESTRICT in2, const int64_t *const LS_RESTRICT in) {
	output[ 0] =		 ((int64_t)((int32_t)in2[0])) * ((int32_t)in[0]);
	output[ 1] =		 ((int64_t)((int32_t)in2[0])) * ((int32_t)in[1]) +
						 ((int64_t)((int32_t)in2[1])) * ((int32_t)in[0]);
	output[ 2] =	2 *  ((int64_t)((int32_t)in2[1])) * ((int32_t)in[1]) +
						 ((int64_t)((int32_t)in2[0])) * ((int32_t)in[2]) +
						 ((int64_t)((int32_t)in2[2])) * ((int32_t)in[0]);
	output[ 3] =		 ((int64_t)((int32_t)in2[1])) * ((int32_t)in[2]) +
						 ((int64_t)((int32_t)in2[2])) * ((int32_t)in[1]) +
						 ((int64_t)((int32_t)in2[0])) * ((int32_t)in[3]) +
						 ((int64_t)((int32_t)in2[3])) * ((int32_t)in[0]);
	output[ 4] =		 ((int64_t)((int32_t)in2[2])) * ((int32_t)in[2]) +
					2 * (((int64_t)((int32_t)in2[1])) * ((int32_t)in[3]) +
						 ((int64_t)((int32_t)in2[3])) * ((int32_t)in[1])) +
						 ((int64_t)((int32_t)in2[0])) * ((int32_t)in[4]) +
						 ((int64_t)((int32_t)in2[4])) * ((int32_t)in[0]);
	output[ 5] =		 ((int64_t)((int32_t)in2[2])) * ((int32_t)in[3]) +
						 ((int64_t)((int32_t)in2[3])) * ((int32_t)in[2]) +
						 ((int64_t)((int32_t)in2[1])) * ((int32_t)in[4]) +
						 ((int64_t)((int32_t)in2[4])) * ((int32_t)in[1]) +
						 ((int64_t)((int32_t)in2[0])) * ((int32_t)in[5]) +
						 ((int64_t)((int32_t)in2[5])) * ((int32_t)in[0]);
	output[ 6] =	2 * (((int64_t)((int32_t)in2[3])) * ((int32_t)in[3]) +
						 ((int64_t)((int32_t)in2[1])) * ((int32_t)in[5]) +
						 ((int64_t)((int32_t)in2[5])) * ((int32_t)in[1])) +
						 ((int64_t)((int32_t)in2[2])) * ((int32_t)in[4]) +
						 ((int64_t)((int32_t)in2[4])) * ((int32_t)in[2]) +
						 ((int64_t)((int32_t)in2[0])) * ((int32_t)in[6]) +
						 ((int64_t)((int32_t)in2[6])) * ((int32_t)in[0]);
	output[ 7] =		 ((int64_t)((int32_t)in2[3])) * ((int32_t)in[4]) +
						 ((int64_t)((int32_t)in2[4])) * ((int32_t)in[3]) +
						 ((int64_t)((int32_t)in2[2])) * ((int32_t)in[5]) +
						 ((int64_t)((int32_t)in2[5])) * ((int32_t)in[2]) +
						 ((int64_t)((int32_t)in2[1])) * ((int32_t)in[6]) +
						 ((int64_t)((int32_t)in2[6])) * ((int32_t)in[1]) +
						 ((int64_t)((int32_t)in2[0])) * ((int32_t)in[7]) +
						 ((int64_t)((int32_t)in2[7])) * ((int32_t)in[0]);
	output[ 8] =		 ((int64_t)((int32_t)in2[4])) * ((int32_t)in[4]) +
					2 * (((int64_t)((int32_t)in2[3])) * ((int32_t)in[5]) +
						 ((int64_t)((int32_t)in2[5])) * ((int32_t)in[3]) +
						 ((int64_t)((int32_t)in2[1])) * ((int32_t)in[7]) +
						 ((int64_t)((int32_t)in2[7])) * ((int32_t)in[1])) +
						 ((int64_t)((int32_t)in2[2])) * ((int32_t)in[6]) +
						 ((int64_t)((int32_t)in2[6])) * ((int32_t)in[2]) +
						 ((int64_t)((int32_t)in2[0])) * ((int32_t)in[8]) +
						 ((int64_t)((int32_t)in2[8])) * ((int32_t)in[0]);
	output[ 9] =		 ((int64_t)((int32_t)in2[4])) * ((int32_t)in[5]) +
						 ((int64_t)((int32_t)in2[5])) * ((int32_t)in[4]) +
						 ((int64_t)((int32_t)in2[3])) * ((int32_t)in[6]) +
						 ((int64_t)((int32_t)in2[6])) * ((int32_t)in[3]) +
						 ((int64_t)((int32_t)in2[2])) * ((int32_t)in[7]) +
						 ((int64_t)((int32_t)in2[7])) * ((int32_t)in[2]) +
						 ((int64_t)((int32_t)in2[1])) * ((int32_t)in[8]) +
						 ((int64_t)((int32_t)in2[8])) * ((int32_t)in[1]) +
						 ((int64_t)((int32_t)in2[0])) * ((int32_t)in[9]) +
						 ((int64_t)((int32_t)in2[9])) * ((int32_t)in[0]);
	output[10] =	2 * (((int64_t)((int32_t)in2[5])) * ((int32_t)in[5]) +
						 ((int64_t)((int32_t)in2[3])) * ((int32_t)in[7]) +
						 ((int64_t)((int32_t)in2[7])) * ((int32_t)in[3]) +
						 ((int64_t)((int32_t)in2[1])) * ((int32_t)in[9]) +
						 ((int64_t)((int32_t)in2[9])) * ((int32_t)in[1])) +
						 ((int64_t)((int32_t)in2[4])) * ((int32_t)in[6]) +
						 ((int64_t)((int32_t)in2[6])) * ((int32_t)in[4]) +
						 ((int64_t)((int32_t)in2[2])) * ((int32_t)in[8]) +
						 ((int64_t)((int32_t)in2[8])) * ((int32_t)in[2]);
	output[11] =		 ((int64_t)((int32_t)in2[5])) * ((int32_t)in[6]) +
						 ((int64_t)((int32_t)in2[6])) * ((int32_t)in[5]) +
						 ((int64_t)((int32_t)in2[4])) * ((int32_t)in[7]) +
						 ((int64_t)((int32_t)in2[7])) * ((int32_t)in[4]) +
						 ((int64_t)((int32_t)in2[3])) * ((int32_t)in[8]) +
						 ((int64_t)((int32_t)in2[8])) * ((int32_t)in[3]) +
						 ((int64_t)((int32_t)in2[2])) * ((int32_t)in[9]) +
						 ((int64_t)((int32_t)in2[9])) * ((int32_t)in[2]);
	output[12] =		 ((int64_t)((int32_t)in2[6])) * ((int32_t)in[6]) +
					2 * (((int64_t)((int32_t)in2[5])) * ((int32_t)in[7]) +
						 ((int64_t)((int32_t)in2[7])) * ((int32_t)in[5]) +
						 ((int64_t)((int32_t)in2[3])) * ((int32_t)in[9]) +
						 ((int64_t)((int32_t)in2[9])) * ((int32_t)in[3])) +
						 ((int64_t)((int32_t)in2[4])) * ((int32_t)in[8]) +
						 ((int64_t)((int32_t)in2[8])) * ((int32_t)in[4]);
	output[13] =		 ((int64_t)((int32_t)in2[6])) * ((int32_t)in[7]) +
						 ((int64_t)((int32_t)in2[7])) * ((int32_t)in[6]) +
						 ((int64_t)((int32_t)in2[5])) * ((int32_t)in[8]) +
						 ((int64_t)((int32_t)in2[8])) * ((int32_t)in[5]) +
						 ((int64_t)((int32_t)in2[4])) * ((int32_t)in[9]) +
						 ((int64_t)((int32_t)in2[9])) * ((int32_t)in[4]);
	output[14] =	2 * (((int64_t)((int32_t)in2[7])) * ((int32_t)in[7]) +
						 ((int64_t)((int32_t)in2[5])) * ((int32_t)in[9]) +
						 ((int64_t)((int32_t)in2[9])) * ((int32_t)in[5])) +
						 ((int64_t)((int32_t)in2[6])) * ((int32_t)in[8]) +
						 ((int64_t)((int32_t)in2[8])) * ((int32_t)in[6]);
	output[15] =		 ((int64_t)((int32_t)in2[7])) * ((int32_t)in[8]) +
						 ((int64_t)((int32_t)in2[8])) * ((int32_t)in[7]) +
						 ((int64_t)((int32_t)in2[6])) * ((int32_t)in[9]) +
						 ((int64_t)((int32_t)in2[9])) * ((int32_t)in[6]);
	output[16] =		 ((int64_t)((int32_t)in2[8])) * ((int32_t)in[8]) +
					2 * (((int64_t)((int32_t)in2[7])) * ((int32_t)in[9]) +
						 ((int64_t)((int32_t)in2[9])) * ((int32_t)in[7]));
	output[17] =		 ((int64_t)((int32_t)in2[8])) * ((int32_t)in[9]) +
						 ((int64_t)((int32_t)in2[9])) * ((int32_t)in[8]);
	output[18] =	2 *  ((int64_t)((int32_t)in2[9])) * ((int32_t)in[9]);
}


void
static freduce_degree(int64_t *const output) {
	output[8] += (output[18] << 4);
	output[8] += (output[18] << 1);
	output[8] += (output[18]     );
	output[7] += (output[17] << 4);
	output[7] += (output[17] << 1);
	output[7] += (output[17]     );
	output[6] += (output[16] << 4);
	output[6] += (output[16] << 1);
	output[6] += (output[16]     );
	output[5] += (output[15] << 4);
	output[5] += (output[15] << 1);
	output[5] += (output[15]     );
	output[4] += (output[14] << 4);
	output[4] += (output[14] << 1);
	output[4] += (output[14]     );
	output[3] += (output[13] << 4);
	output[3] += (output[13] << 1);
	output[3] += (output[13]     );
	output[2] += (output[12] << 4);
	output[2] += (output[12] << 1);
	output[2] += (output[12]     );
	output[1] += (output[11] << 4);
	output[1] += (output[11] << 1);
	output[1] += (output[11]     );
	output[0] += (output[10] << 4);
	output[0] += (output[10] << 1);
	output[0] += (output[10]     );
}


int64_t
static LS_ATTR_INLINE div_by_2_26(const int64_t v) {
	const uint32_t highword = (uint32_t)(((uint64_t)v) >> 32);
	const int32_t sign = (((int32_t)highword) >> 31);
	const int32_t roundoff = (((uint32_t)sign) >> 6);

	return ((v + roundoff) >> 26);
}


int64_t
static LS_ATTR_INLINE div_by_2_25(const int64_t v) {
	const uint32_t highword = (uint32_t)(((uint64_t)v) >> 32);
	const int32_t sign = (((int32_t)highword) >> 31);
	const int32_t roundoff = (((uint32_t)sign) >> 7);

	return ((v + roundoff) >> 25);
}


void
static freduce_coefficients(int64_t *const output) {
	ls_nword_t i;
	register int64_t over;

	output[10] = 0;

	for (i = 0; i < 10; i += 2) {
		over = div_by_2_26(output[i]);
		output[i] -= (over << 26);
		output[i + 1] += over;

		over = div_by_2_25(output[i + 1]);
		output[i + 1] -= (over << 25);
		output[i + 2] += over;
	}

	output[0] += (output[10] << 4);
	output[0] += (output[10] << 1);
	output[0] += (output[10]     );
	output[10] = 0;

	over = div_by_2_26(output[0]);
	output[0] -= (over << 26);
	output[1] += over;
}


void
static fmul(int64_t *const LS_RESTRICT output, const int64_t *const LS_RESTRICT in, const int64_t *const LS_RESTRICT in2) {
	int64_t t[19];

	fproduct(t, in, in2);
	freduce_degree(t);
	freduce_coefficients(t);

	memcpy(output, t, INTARR_SIZE);
}


void
static fsquare(int64_t *const LS_RESTRICT output, const int64_t *const LS_RESTRICT in) {
	int64_t t[19];

	t[ 0] =		( (int64_t)((int32_t)in[0])) * ((int32_t)in[0]);
	t[ 1] =	2 * ( (int64_t)((int32_t)in[0])) * ((int32_t)in[1]);
	t[ 2] =	2 * (((int64_t)((int32_t)in[1])) * ((int32_t)in[1]) +
				( (int64_t)((int32_t)in[0])) * ((int32_t)in[2]));
	t[ 3] =	2 * (((int64_t)((int32_t)in[1])) * ((int32_t)in[2]) +
				( (int64_t)((int32_t)in[0])) * ((int32_t)in[3]));
	t[ 4] =		( (int64_t)((int32_t)in[2])) * ((int32_t)in[2]) +
			4 * ( (int64_t)((int32_t)in[1])) * ((int32_t)in[3]) +
			2 * ( (int64_t)((int32_t)in[0])) * ((int32_t)in[4]);
	t[ 5] =	2 * (((int64_t)((int32_t)in[2])) * ((int32_t)in[3]) +
				( (int64_t)((int32_t)in[1])) * ((int32_t)in[4]) +
				( (int64_t)((int32_t)in[0])) * ((int32_t)in[5]));
	t[ 6] =	2 * (((int64_t)((int32_t)in[3])) * ((int32_t)in[3]) +
				( (int64_t)((int32_t)in[2])) * ((int32_t)in[4]) +
				( (int64_t)((int32_t)in[0])) * ((int32_t)in[6]) +
			2 * ( (int64_t)((int32_t)in[1])) * ((int32_t)in[5]));
	t[ 7] =	2 * (((int64_t)((int32_t)in[3])) * ((int32_t)in[4]) +
				( (int64_t)((int32_t)in[2])) * ((int32_t)in[5]) +
				( (int64_t)((int32_t)in[1])) * ((int32_t)in[6]) +
				( (int64_t)((int32_t)in[0])) * ((int32_t)in[7]));
	t[ 8] =		( (int64_t)((int32_t)in[4])) * ((int32_t)in[4]) +
			2 * (((int64_t)((int32_t)in[2])) * ((int32_t)in[6]) +
				( (int64_t)((int32_t)in[0])) * ((int32_t)in[8]) +
			2 * (((int64_t)((int32_t)in[1])) * ((int32_t)in[7]) +
				( (int64_t)((int32_t)in[3])) * ((int32_t)in[5])));
	t[ 9] =	2 * (((int64_t)((int32_t)in[4])) * ((int32_t)in[5]) +
				( (int64_t)((int32_t)in[3])) * ((int32_t)in[6]) +
				( (int64_t)((int32_t)in[2])) * ((int32_t)in[7]) +
				( (int64_t)((int32_t)in[1])) * ((int32_t)in[8]) +
				( (int64_t)((int32_t)in[0])) * ((int32_t)in[9]));
	t[10] =	2 * (((int64_t)((int32_t)in[5])) * ((int32_t)in[5]) +
				( (int64_t)((int32_t)in[4])) * ((int32_t)in[6]) +
				( (int64_t)((int32_t)in[2])) * ((int32_t)in[8]) +
			2 * (((int64_t)((int32_t)in[3])) * ((int32_t)in[7]) +
				( (int64_t)((int32_t)in[1])) * ((int32_t)in[9])));
	t[11] =	2 * (((int64_t)((int32_t)in[5])) * ((int32_t)in[6]) +
				( (int64_t)((int32_t)in[4])) * ((int32_t)in[7]) +
				( (int64_t)((int32_t)in[3])) * ((int32_t)in[8]) +
				( (int64_t)((int32_t)in[2])) * ((int32_t)in[9]));
	t[12] =		( (int64_t)((int32_t)in[6])) * ((int32_t)in[6]) +
			2 * (((int64_t)((int32_t)in[4])) * ((int32_t)in[8]) +
			2 * (((int64_t)((int32_t)in[5])) * ((int32_t)in[7]) +
				( (int64_t)((int32_t)in[3])) * ((int32_t)in[9])));
	t[13] =	2 * (((int64_t)((int32_t)in[6])) * ((int32_t)in[7]) +
				( (int64_t)((int32_t)in[5])) * ((int32_t)in[8]) +
				( (int64_t)((int32_t)in[4])) * ((int32_t)in[9]));
	t[14] =	2 * (((int64_t)((int32_t)in[7])) * ((int32_t)in[7]) +
				( (int64_t)((int32_t)in[6])) * ((int32_t)in[8]) +
			2 * ( (int64_t)((int32_t)in[5])) * ((int32_t)in[9]));
	t[15] =	2 * (((int64_t)((int32_t)in[7])) * ((int32_t)in[8]) +
				( (int64_t)((int32_t)in[6])) * ((int32_t)in[9]));
	t[16] =		( (int64_t)((int32_t)in[8])) * ((int32_t)in[8]) +
			4 * ( (int64_t)((int32_t)in[7])) * ((int32_t)in[9]);
	t[17] =	2 * ( (int64_t)((int32_t)in[8])) * ((int32_t)in[9]);
	t[18] =	2 * ( (int64_t)((int32_t)in[9])) * ((int32_t)in[9]);

	freduce_degree(t);
	freduce_coefficients(t);

	memcpy(output, t, INTARR_SIZE);
}


void
static LS_ATTR_INLINE fexpand(int64_t *const LS_RESTRICT output, const uint8_t *const LS_RESTRICT input) {
#define F(n, s, shift, mask) {					\
	output[n] = (((								\
		(((int64_t)input[((s) + 0)])      ) |	\
		(((int64_t)input[((s) + 1)]) <<  8) |	\
		(((int64_t)input[((s) + 2)]) << 16) |	\
		(((int64_t)input[((s) + 3)]) << 24)		\
	) >> shift) & mask);						\
}

	F(0,  0, 0, 0x03FFFFFF);
	F(1,  3, 2, 0x01FFFFFF);
	F(2,  6, 3, 0x03FFFFFF);
	F(3,  9, 5, 0x01FFFFFF);
	F(4, 12, 6, 0x03FFFFFF);
	F(5, 16, 0, 0x01FFFFFF);
	F(6, 19, 1, 0x03FFFFFF);
	F(7, 22, 3, 0x01FFFFFF);
	F(8, 25, 4, 0x03FFFFFF);
	F(9, 28, 6, 0x01FFFFFF);

#undef F
}


void
static LS_ATTR_INLINE fcontract(uint8_t *output, int64_t *input_limbs) {
	int32_t input[10];
	int32_t mask;

	ls_nword_t i, j;

	for (i = 10; i--;) {
		input[i] = (int32_t)input_limbs[i];
	}

	for (j = 2; j--;) {
		for (i = 0; i < 9; ++i) {
			if ((i & 1) == 1) {
				const int32_t carry = -((input[i] & (input[i] >> 31)) >> 25);
				input[i] += (carry << 25);
				input[i + 1] -= carry;
			} else {
				const int32_t carry = -((input[i] & (input[i] >> 31)) >> 26);
				input[i] += (carry << 26);
				input[i + 1] -= carry;
			}
		}

		{
			const int32_t carry = -((input[9] & (input[9] >> 31)) >> 25);
			input[9] = (input[9] + (carry << 25));
			input[0] = (input[0] - (carry * 19));
		}
	}

	{
		const int32_t carry = -((input[0] & (input[0] >> 31)) >> 26);
		input[0] = (input[0] + (carry << 26));
		input[1] = (input[1] - carry);
	}

	for (j = 2; j--;) {
		for (i = 0; i < 9; i++) {
			if ((i & 1) == 1) {
				input[i + 1] += (input[i] >> 25);
				input[i] &= 0x01FFFFFF;
			} else {
				input[i + 1] += (input[i] >> 26);
				input[i] &= 0x03FFFFFF;
			}
		}

		input[0] += (19 * (input[9] >> 25));
		input[9] &= 0x01FFFFFF;
	}

	mask = ~((input[0] - 0x03FFFFED) >> 31);

	{
		int32_t a;
		for (i = 10; --i;) {
			a = ~(input[i] ^ (((i & 1) == 1) ? 0x01FFFFFF : 0x03FFFFFF));
			a &= (a << 16);
			a &= (a << 8);
			a &= (a << 4);
			a &= (a << 2);
			a &= (a << 1);
			mask &= (a >> 31);
		}
	}

	for (i = 10; --i;) {
		input[i] -= (mask & (((i & 1) == 1) ? 0x01FFFFFF : 0x03FFFFFF));
	}

	input[0] -= (mask & 0x03FFFFED);
	input[1] <<= 2;
	input[2] <<= 3;
	input[3] <<= 5;
	input[4] <<= 6;
	input[6] <<= 1;
	input[7] <<= 3;
	input[8] <<= 4;
	input[9] <<= 6;

	output[ 0] = 0;
	output[16] = 0;

#define F(i, s) {									\
	output[((s) + 0)] |= ((input[i]      ) & 0xFF); \
	output[((s) + 1)]  = ((input[i] >>  8) & 0xFF); \
	output[((s) + 2)]  = ((input[i] >> 16) & 0xFF); \
	output[((s) + 3)]  = ((input[i] >> 24) & 0xFF); \
}

	F(0,  0);
	F(1,  3);
	F(2,  6);
	F(3,  9);
	F(4, 12);
	F(5, 16);
	F(6, 19);
	F(7, 22);
	F(8, 25);
	F(9, 28);

#undef F
}


void
static LS_ATTR_INLINE fmonty(int64_t *x2, int64_t *z2, int64_t *x3, int64_t *z3, int64_t *x, int64_t *z, int64_t *xprime, int64_t *zprime, const int64_t *qmqp) {
	int64_t
		origx[10],
		origxprime[10],
		zzz[19],
		xx[19],
		zz[19],
		xxprime[19],
		zzprime[19],
		zzzprime[19],
		xxxprime[19];

	memcpy(origx, x, 10 * sizeof(int64_t));
	fsum(x, z);
	fdifference(z, origx);

	memcpy(origxprime, xprime, INTARR_SIZE);
	fsum(xprime, zprime);
	fdifference(zprime, origxprime);
	fproduct(xxprime, xprime, z);
	fproduct(zzprime, x, zprime);

	freduce_degree(xxprime);
	freduce_coefficients(xxprime);
	freduce_degree(zzprime);
	freduce_coefficients(zzprime);

	memcpy(origxprime, xxprime, INTARR_SIZE);
	fsum(xxprime, zzprime);
	fdifference(zzprime, origxprime);
	fsquare(xxxprime, xxprime);
	fsquare(zzzprime, zzprime);
	fproduct(zzprime, zzzprime, qmqp);
	freduce_degree(zzprime);
	freduce_coefficients(zzprime);

	memcpy(x3, xxxprime, INTARR_SIZE);
	memcpy(z3, zzprime, INTARR_SIZE);

	fsquare(xx, x);
	fsquare(zz, z);
	fproduct(x2, xx, zz);

	freduce_degree(x2);
	freduce_coefficients(x2);
	fdifference(zz, xx);

	memset(zzz + 10, 0, INTARR_SIZE - sizeof(int64_t));

	ls_nword_t i;
	for (i = 10; i--;) {
		zzz[i] = (zz[i] * 0x0001DB41);
	}

	freduce_coefficients(zzz);
	fsum(zzz, xx);
	fproduct(z2, zz, zzz);
	freduce_degree(z2);
	freduce_coefficients(z2);
}


void
static LS_ATTR_INLINE swap_conditional(int64_t a[19], int64_t b[19], ls_nword_t iswap) {
	const int32_t swap = ((int32_t)-iswap);

	ls_nword_t i;
	for (i = 10; i--;) {
		const int32_t x = (swap & (((int32_t)a[i]) ^ ((int32_t)b[i])));
		a[i] = (((int32_t)a[i]) ^ x);
		b[i] = (((int32_t)b[i]) ^ x);
	}
}


void
static LS_ATTR_INLINE cmult(int64_t *resultx, int64_t *resultz, const uint8_t *n, const int64_t *q) {
	int64_t
		a[19] = { 0 },
		b[19] = { 1 },
		c[19] = { 1 },
		d[19] = { 0 },
		e[19] = { 0 },
		f[19] = { 1 },
		g[19] = { 0 },
		h[19] = { 1 },
		*nqpqx = a,
		*nqpqz = b,
		*nqx = c,
		*nqz = d,
		*nqpqx2 = e,
		*nqpqz2 = f,
		*nqx2 = g,
		*nqz2 = h,
		*t = NULL;

	memcpy(nqpqx, q, INTARR_SIZE);

	ls_nword_t i, j;
#if (LS_CURVE25519_REVERSE)
	for (i = 32; i--;)
#else
	for (i = 0; i < 32; ++i)
#endif
	{
		uint8_t byte = n[31 - i];
		for (j = 8; j--;) {
			const ls_nword_t bit = (byte >> 7);

			swap_conditional(nqx, nqpqx, bit);
			swap_conditional(nqz, nqpqz, bit);
			fmonty(nqx2, nqz2, nqpqx2, nqpqz2, nqx, nqz, nqpqx, nqpqz, q);
			swap_conditional(nqx2, nqpqx2, bit);
			swap_conditional(nqz2, nqpqz2, bit);

			t = nqx;
			nqx = nqx2;
			nqx2 = t;
			t = nqz;
			nqz = nqz2;
			nqz2 = t;
			t = nqpqx;
			nqpqx = nqpqx2;
			nqpqx2 = t;
			t = nqpqz;
			nqpqz = nqpqz2;
			nqpqz2 = t;

			byte <<= 1;
		}
	}

	memcpy(resultx, nqx, INTARR_SIZE);
	memcpy(resultz, nqz, INTARR_SIZE);
}


void
static crecip(int64_t *out, const int64_t *z_in) {
	ls_nword_t i;

	int64_t
		buffer_a[10],
		buffer_b[10],
		z[10],
		t0[10],
		t1[10];

	fsquare(buffer_a, z_in);
	fsquare(t1, buffer_a);
	fsquare(t0, t1);
	fmul(buffer_b, t0, z_in);
	fmul(z, buffer_b, buffer_a);
	fsquare(t0, z);
	fmul(buffer_a, t0, buffer_b);

	fsquare(t0, buffer_a);
	fsquare(t1, t0);
	fsquare(t0, t1);
	fsquare(t1, t0);
	fsquare(t0, t1);
	fmul(buffer_b, t0, buffer_a);

	fsquare(t0, buffer_b);
	fsquare(t1, t0);
	fsquare(t0, t1);
	fsquare(t1, t0);
	fsquare(t0, t1);
	fsquare(t1, t0);
	fsquare(t0, t1);
	fsquare(t1, t0);
	fsquare(t0, t1);
	fsquare(t1, t0);
	fmul(buffer_a, t1, buffer_b);

	fsquare(t0, buffer_a);
	fsquare(t1, t0);
	fsquare(t0, t1);
	fsquare(t1, t0);
	for (i = 4; i--;) {
		fsquare(t0, t1);
		fsquare(t1, t0);
		fsquare(t0, t1);
		fsquare(t1, t0);
	}
	fmul(t0, t1, buffer_a);

	fsquare(t1, t0);
	fsquare(t0, t1);
	fsquare(t1, t0);
	fsquare(t0, t1);
	fsquare(t1, t0);
	fsquare(t0, t1);
	fsquare(t1, t0);
	fsquare(t0, t1);
	fsquare(t1, t0);
	fsquare(t0, t1);
	fmul(buffer_a, t0, buffer_b);

	fsquare(t0, buffer_a);
	fsquare(t1, t0);
	for (i = 12; i--;) {
		fsquare(t0, t1);
		fsquare(t1, t0);
		fsquare(t0, t1);
		fsquare(t1, t0);
	}
	fmul(buffer_b, t1, buffer_a);

	fsquare(t1, buffer_b);
	fsquare(t0, t1);
	fsquare(t1, t0);
	fsquare(t0, t1);
	for (i = 12; i--;) {
		fsquare(t1, t0);
		fsquare(t0, t1);
		fsquare(t1, t0);
		fsquare(t0, t1);
		fsquare(t1, t0);
		fsquare(t0, t1);
		fsquare(t1, t0);
		fsquare(t0, t1);
	}
	fmul(t1, t0, buffer_b);

	fsquare(t0, t1);
	fsquare(t1, t0);
	for (i = 12; i--;) {
		fsquare(t0, t1);
		fsquare(t1, t0);
		fsquare(t0, t1);
		fsquare(t1, t0);
	}
	fmul(t0, t1, buffer_a);

	fsquare(t1, t0);
	fsquare(t0, t1);
	fsquare(t1, t0);
	fsquare(t0, t1);
	fsquare(t1, t0);
	fmul(out, t1, z);
}


ls_result_t
ls_curve25519(uint8_t out[32], const uint8_t x[32], const uint8_t y[32]) {
	LS_RESULT_CHECK_NULL(out, 1);
	LS_RESULT_CHECK_NULL(x, 2);
	LS_RESULT_CHECK_NULL(y, 3);

	int64_t
		a[10],
		z[11];

	{
		uint8_t e[32];
		int64_t bp[10];

		memcpy(e, x, 32);
		e[0] &= 248;
		e[31] = ((e[31] & 0x7F) | 0x40);

		fexpand(bp, y);
		cmult(a, z, e, bp);
	}

	int64_t zmone[10];

	crecip(zmone, z);
	fmul(z, a, zmone);
	fcontract(out, z);

	return LS_RESULT_SUCCESS;
}


#	else
#		error "This code only works when >> does sign-extension on negative numbers."
#	endif
#else
#	error "This code only works on a two's complement system."
#endif
