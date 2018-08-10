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
*******************************************************************************/


#undef LS_EXPORTING

#include "./unit/bitops.h"

#include "../libserum/io/log.h"

#include "../libserum/crypto/__selftests/base.h"
#include "../libserum/crypto/registry.h"

#include <string.h>




int __run_siot_server();
int __run_siot_client();


int
__run_cst() {
	lscst_set_logging(true);
	const ls_result_t cst_result = lscst_launch();

#if (LSCST_ENABLED)
	return ((cst_result == LS_E_SUCCESS) ? 0 : 1);
#else
	return ((cst_result == LS_E_UNSUPPORTED) ? 0 : 1);
#endif
}




int
__run_unit_tests() {
	int(*tests[])(void) = {
		lst_unit_bitops
	};

	int failed = 0;

	unsigned int i;
	for (i = (sizeof(tests) / sizeof(*tests)); i--;) {
		if (tests[i] != NULL) {
			if (tests[i]() != 0) {
				++failed;
			}
		}
	}

	return failed;
}




int
main(int argc, char *argv[], char *env[]) {
	if (argc < 2) {
		return 1;
	}


	/* Initialize the logger ourselves, so we send everything to stderr. */
	ls_log_init_ex(NULL, 0, LS_LOG_LEVEL_DEFAULT, stderr);


	const struct {
		char param[32];
		int (*function)();
	} __paramtable[] = {
		{
			.param = "run-cst",
			.function = __run_cst
		},
		{
			.param = "unit-test",
			.function = __run_unit_tests
		},
		{
			.param = "siot-server",
			.function = __run_siot_server
		},
		{
			.param = "siot-client",
			.function = __run_siot_client
		}
	};
	const size_t __paramtable_count = (sizeof(__paramtable) / sizeof(*__paramtable));


	const size_t param_len = strlen(argv[1]);
	typeof(*__paramtable) *ptmp;

	size_t i;
	for (i = __paramtable_count; i--;) {
		ptmp = &__paramtable[i];

		if (strlen(ptmp->param) != param_len) {
			continue;
		}

		if (memcmp(ptmp->param, argv[1], param_len) != 0) {
			continue;
		}

		return ptmp->function();
	}


	return 2;
}
