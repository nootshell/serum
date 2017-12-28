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

#define FILE_PATH							"crypto/kdf/self-test.c"

#include "../../core/stdincl.h"
#include "../../debug/__self-test_logging.h"
#include "./_signatures.h"
#include "../../core/memory.h"


static const char *vectors[] = {
	"p='' s='' i=16 w=1 o=1 os=64",
	"p='password' s='NaCl' i=1024 w=8 o=16 os=64",
	"p='pleaseletmein' s='SodiumChloride' i=16384 w=8 o=1 os=64",
	"p='pleaseletmein' s='SodiumChloride' i=1048576 w=8 o=1 os=64",
	NULL
};

static const char *vectors_pass[] = {
	"",
	"password",
	"pleaseletmein",
	"pleaseletmein"
};

static const char *vectors_salt[] = {
	"",
	"NaCl",
	"SodiumChloride",
	"SodiumChloride"
};

static const ls_nword_t vectors_rnp[][3] = {
	{ 16, 1, 1 },
	{ 1024, 8, 16 },
	{ 16384, 8, 1 },
	{ 1048576, 8, 1 }
};


#include "./scrypt-sha2.h"

static const uint8_t output_scrypt_sha2_256[][64] = {
	{ 0x77, 0xD6, 0x57, 0x62, 0x38, 0x65, 0x7B, 0x20, 0x3B, 0x19, 0xCA, 0x42, 0xC1, 0x8A, 0x04, 0x97, 0xF1, 0x6B, 0x48, 0x44, 0xE3, 0x07, 0x4A, 0xE8, 0xDF, 0xDF, 0xFA, 0x3F, 0xED, 0xE2, 0x14, 0x42, 0xFC, 0xD0, 0x06, 0x9D, 0xED, 0x09, 0x48, 0xF8, 0x32, 0x6A, 0x75, 0x3A, 0x0F, 0xC8, 0x1F, 0x17, 0xE8, 0xD3, 0xE0, 0xFB, 0x2E, 0x0D, 0x36, 0x28, 0xCF, 0x35, 0xE2, 0x0C, 0x38, 0xD1, 0x89, 0x06 },
	{ 0xFD, 0xBA, 0xBE, 0x1C, 0x9D, 0x34, 0x72, 0x00, 0x78, 0x56, 0xE7, 0x19, 0x0D, 0x01, 0xE9, 0xFE, 0x7C, 0x6A, 0xD7, 0xCB, 0xC8, 0x23, 0x78, 0x30, 0xE7, 0x73, 0x76, 0x63, 0x4B, 0x37, 0x31, 0x62, 0x2E, 0xAF, 0x30, 0xD9, 0x2E, 0x22, 0xA3, 0x88, 0x6F, 0xF1, 0x09, 0x27, 0x9D, 0x98, 0x30, 0xDA, 0xC7, 0x27, 0xAF, 0xB9, 0x4A, 0x83, 0xEE, 0x6D, 0x83, 0x60, 0xCB, 0xDF, 0xA2, 0xCC, 0x06, 0x40 },
	{ 0x70, 0x23, 0xBD, 0xCB, 0x3A, 0xFD, 0x73, 0x48, 0x46, 0x1C, 0x06, 0xCD, 0x81, 0xFD, 0x38, 0xEB, 0xFD, 0xA8, 0xFB, 0xBA, 0x90, 0x4F, 0x8E, 0x3E, 0xA9, 0xB5, 0x43, 0xF6, 0x54, 0x5D, 0xA1, 0xF2, 0xD5, 0x43, 0x29, 0x55, 0x61, 0x3F, 0x0F, 0xCF, 0x62, 0xD4, 0x97, 0x05, 0x24, 0x2A, 0x9A, 0xF9, 0xE6, 0x1E, 0x85, 0xDC, 0x0D, 0x65, 0x1E, 0x40, 0xDF, 0xCF, 0x01, 0x7B, 0x45, 0x57, 0x58, 0x87 },
	{ 0x21, 0x01, 0xCB, 0x9B, 0x6A, 0x51, 0x1A, 0xAE, 0xAD, 0xDB, 0xBE, 0x09, 0xCF, 0x70, 0xF8, 0x81, 0xEC, 0x56, 0x8D, 0x57, 0x4A, 0x2F, 0xFD, 0x4D, 0xAB, 0xE5, 0xEE, 0x98, 0x20, 0xAD, 0xAA, 0x47, 0x8E, 0x56, 0xFD, 0x8F, 0x4B, 0xA5, 0xD0, 0x9F, 0xFA, 0x1C, 0x6D, 0x92, 0x7C, 0x40, 0xF4, 0xC3, 0x37, 0x30, 0x40, 0x49, 0xE8, 0xA9, 0x52, 0xFB, 0xCB, 0xF4, 0x5C, 0x6F, 0xA7, 0x7A, 0x41, 0xA4 }
};


ls_result_t
ls_test_crypto_kdf_ex(ls_kdf_func_ex_t kdf, const char *pass, const size_t pass_size, const char *salt, const size_t salt_size, const ls_nword_t inner_rounds, const ls_nword_t weight, const ls_nword_t outer_rounds, const uint8_t *sample, const size_t sample_size) {
	LS_RESULT_CHECK_NULL(kdf, 1);
	LS_RESULT_CHECK_NULL(pass, 2);
	LS_RESULT_CHECK_NULL(salt, 3);
	LS_RESULT_CHECK_NULL(sample, 4);

#if (LS_KDF_DENY_PASS_ZERO_SIZE)
	LS_RESULT_CHECK_SIZE(pass_size, 1);
#endif
#if (LS_KDF_DENY_SALT_ZERO_SIZE)
	LS_RESULT_CHECK_SIZE(salt_size, 2);
#endif
	LS_RESULT_CHECK_SIZE(inner_rounds, 3);
	LS_RESULT_CHECK_SIZE(weight, 4);
	LS_RESULT_CHECK_SIZE(outer_rounds, 5);
	LS_RESULT_CHECK_SIZE(sample_size, 6);

	ls_result_t result;

	uint8_t stackalloc(sample_buffer, sample_size);
	if (!(result = kdf(sample_buffer, sample_size, pass, pass_size, salt, salt_size, inner_rounds, weight, outer_rounds)).success) {
		stackfree(sample_buffer);
		return LS_RESULT_INHERITED(result, false);
	}

	if (memcmp(sample_buffer, sample, sample_size) == 0) {
		result = LS_RESULT_SUCCESS;
	} else {
		result = LS_RESULT_ERROR(LS_RESULT_CODE_DATA);
	}

	memset(sample_buffer, 0, sample_size);
	stackfree(sample_buffer);

	return result;
}


ls_bool
ls_selftest_crypto_kdf() {
	ls_bool passed = true;

	ls_nword_t i;
	const char
		*vector = NULL,
		*input_pass = NULL,
		*input_salt = NULL;
	size_t
		pass_size = 0,
		salt_size = 0;
	const ls_nword_t *rnp = NULL;

	START_TEST("cryptographic key derivation functions");

	for (i = 0; ((vector = vectors[i]) && (rnp = vectors_rnp[i]) && (input_pass = vectors_pass[i]) && (input_salt = vectors_salt[i])); ++i) {
		pass_size = strlen(input_pass);
		salt_size = strlen(input_salt);

		START_VECTOR(i, vector);

		if (ls_test_crypto_kdf_ex((ls_kdf_func_ex_t)ls_scrypt_sha2_256, input_pass, pass_size, input_salt, salt_size, rnp[0], rnp[1], rnp[2], output_scrypt_sha2_256[i], sizeof(output_scrypt_sha2_256[i])).success) {
			TEST_SUB_PASSED("scrypt-SHA2-256");
		} else {
			TEST_SUB_FAILED("scrypt-SHA2-256");
			passed = false;
		}
	}

	if (passed) {
		END_TEST_PASSED("cryptographic key derivation functions");
	} else {
		END_TEST_FAILED("cryptographic key derivation functions");
	}

	return passed;
}
