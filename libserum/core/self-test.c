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

#define FILE_PATH							"core/self-test.c"

#include "./self-test.h"

#if (LS_SELFTEST)

#include "./memory.h"
#include "./logging/log.h"

#include "../crypto/hashing/self-test.h"


ID("BIST: global");


struct ls_selftest {
	ls_bool(*func)();
	char description[32];
};

struct ls_selftest tests[] = {
#if (LS_SELFTEST_CRYPTO_HASHING)
	{ ls_selftest_crypto_hashing, "cryptographic hash functions" },
#endif
	{ 0 }
};


ls_bool
ls_selftest_all() {
	const size_t max = ((sizeof(tests) / sizeof(*tests)) - 1);
	ls_nword_t failures = 0;
	struct ls_selftest *current_test;
	struct ls_selftest *stackalloc(failed_entries, max);

	if (max == 0) {
#if (LS_SELFTEST_VERBOSE)
		ls_log(LS_LOG_ERROR, "No self-tests to perform.");
#endif
		return false;
	}

	ls_nword_t i;
	for (i = 0; i < max; ++i) {
		current_test = &tests[i];
		failed_entries[i] = NULL;
		if (current_test->func) {
			if (!current_test->func()) {
				failed_entries[i] = current_test;
				++failures;
			}
		}
	}

	if (!failures) {
#if (LS_SELFTEST_VERBOSE)
		ls_log(LS_LOG_VERBOSE, "All tests passed.");
#endif
		return true;
	} else {
		if (failures == max) {
			ls_log(LS_LOG_ERROR, "All tests failed:");
		} else {
			ls_log(LS_LOG_ERROR, "Out of %u test%s, %u test%s failed:", max, ((max == 1) ? "" : "s"), failures, ((failures == 1) ? "" : "s"));
		}
		for (i = 0; i < max; ++i) {
			if (failed_entries[i] != NULL) {
				ls_log(LS_LOG_ERROR, "  %s", failed_entries[i]->description);
			}
		}
	}

	return false;
}

#endif
