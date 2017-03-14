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

#include <libserum/crypto/hashing/sha2.h>

struct ls_tc_sha2 {
	int type;
	union {
		ls_sha2_224_t s224;
		ls_sha2_256_t s256;
		ls_sha2_384_t s384;
		ls_sha2_512_t s512;
	} data;
};

int ls_tc_sha2_init(void *data) {
	struct ls_tc_sha2 *ctx = data;

	switch (ctx->type) {
		case 224:
			if (!ls_sha2_224_init(&ctx->data.s224).success) {
				return -2;
			}
			break;
		case 256:
			if (!ls_sha2_256_init(&ctx->data.s256).success) {
				return -3;
			}
			break;
		case 384:
			if (!ls_sha2_384_init(&ctx->data.s384).success) {
				return -4;
			}
			break;
		case 512:
			if (!ls_sha2_512_init(&ctx->data.s512).success) {
				return -5;
			}
			break;
		default:
			return -1;
	}

	return 0;
}

int ls_tc_sha2_perform(void *data, void *input, size_t size) {
	struct ls_tc_sha2 *ctx = data;

	switch (ctx->type) {
		case 224:

			break;
		case 256:
			break;
		case 384:
			break;
		case 512:
			break;
		default:
			return -1;
	}

	return 0;
}
