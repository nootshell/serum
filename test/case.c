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

#include "./case.h"


int ls_testcase_init(ls_testcase_t *ctx, void *data, int(*f_init)(void *data), int(*f_perform)(void *data, void *input, size_t input_size), int(*f_clear)(void *data)) {
	if (!ctx || !f_init || !f_perform || !f_clear) {
		return -1;
	}

	ctx->d_data = data;
	ctx->f_init = f_init;
	ctx->f_perform = f_perform;
	ctx->f_clear = f_clear;

	return 0;
}

int ls_testcase_clear(ls_testcase_t *ctx) {
	if (!ctx) {
		return -1;
	}

	ctx->d_data = NULL;
	ctx->f_init = NULL;
	ctx->f_perform = NULL;
	ctx->f_clear = NULL;

	return 0;
}

int ls_testcase_run(ls_testcase_t *ctx, void *input, size_t input_size) {
	if (!ctx || !ctx->d_data || !ctx->f_init || !ctx->f_perform || !ctx->f_clear) {
		return -1;
	}

	int result;

	result = ctx->f_init(ctx->d_data);
	result = ctx->f_perform(ctx->d_data, input, input_size);
	result = ctx->f_clear(ctx->d_data);

	return result;
}
