/*******************************************************************************
 * *                                                                            **
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
 **    Template file, used by sha2-template.c
 **
 */

#ifdef SHA2_UPDATE_BLOCK
ls_result_t
static SHA2_UPDATE_BLOCK(SHA2_CTX *const ctx, const SHA2_NATIVE_TYPE block[LS_SHA2_BLOCK_NUM]) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(block, 2);

	register SHA2_NATIVE_TYPE
		h0 = ctx->h[0],
		h1 = ctx->h[1],
		h2 = ctx->h[2],
		h3 = ctx->h[3],
		h4 = ctx->h[4],
		h5 = ctx->h[5],
		h6 = ctx->h[6],
		h7 = ctx->h[7],
		s0, s1, ch, temp1, temp2, maj;

	SHA2_NATIVE_TYPE w[SHA2_WR];

	register uint_fast16_t i;

	for (i = 16; i--;) {
		w[i] = SHA2_SWAP(block[i]);
	}

	for (i = 16; i < SHA2_WR; ++i) {
		s0 = (SHA2_ROTR(w[(i - 15)], SHA2_ROTR_1) ^ SHA2_ROTR(w[(i - 15)], SHA2_ROTR_2) ^ (w[(i - 15)] >> SHA2_SHR_1));
		s1 = (SHA2_ROTR(w[(i -  2)], SHA2_ROTR_3) ^ SHA2_ROTR(w[(i -  2)], SHA2_ROTR_4) ^ (w[(i -  2)] >> SHA2_SHR_2));
		w[i] = (w[(i - 16)] + s0 + w[(i - 7)] + s1);
	}

	for (i = 0; i < SHA2_WR; ++i) {
		s1 = (SHA2_ROTR(h4, SHA2_ROTR_5) ^ SHA2_ROTR(h4, SHA2_ROTR_6) ^ SHA2_ROTR(h4, SHA2_ROTR_7));
		ch = ((h4 & h5) ^ (~h4 & h6));
		temp1 = (h7 + s1 + ch + SHA2_CONSTANTS[i] + w[i]);

		s0 = (SHA2_ROTR(h0, SHA2_ROTR_8) ^ SHA2_ROTR(h0, SHA2_ROTR_9) ^ SHA2_ROTR(h0, SHA2_ROTR_10));
		maj = ((h0 & h1) ^ (h0 & h2) ^ (h1 & h2));
		temp2 = (s0 + maj);

		h7 = h6;
		h6 = h5;
		h5 = h4;
		h4 = (h3 + temp1);
		h3 = h2;
		h2 = h1;
		h1 = h0;
		h0 = (temp1 + temp2);
	}

	ctx->h[0] += h0;
	ctx->h[1] += h1;
	ctx->h[2] += h2;
	ctx->h[3] += h3;
	ctx->h[4] += h4;
	ctx->h[5] += h5;
	ctx->h[6] += h6;
	ctx->h[7] += h7;

	return LS_RESULT_SUCCESS;
}
#endif
