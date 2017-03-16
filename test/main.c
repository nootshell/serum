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

#define FILE_PATH "main.c"

#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include <libserum/core/varsize.h>
#include <libserum/debug/log.h>
#include <libserum/debug/memdump.h>
#include <libserum/crypto/hashing/sha2.h>

int main(int argc, char *argv[], char *env[]) {
	int ret = 0;

	ls_sha2_224_t sha2;

	if (!ls_sha2_224_init(&sha2).success) {
		ret = 1;
		goto __cleanup;
	}

	/*uint8_t data[4];
	strcpy((void*)data, "abc");
	data[3] = 0;

	if (!ls_sha2_224_update(&sha2, data, 3).success) {
		ret = 2;
		goto __cleanup;
	}*/

	uint8_t digest[LS_SHA2_224_DIGEST_SIZE];
	if (!ls_sha2_224_finish(&sha2, digest).success) {
		ret = 3;
		goto __cleanup;
	}

	ls_memdump(digest, sizeof(digest));

__cleanup:
	ls_sha2_224_clear(&sha2);
	return ret;

	ls_vs_value_t vsv = 123456789;
	if (argc > 1) {
		vsv = (ls_vs_value_t)strtoumax(argv[1], NULL, 10);
	}

	uint8_t vsb[LS_VARSIZE_BUFFER_SIZE]; // VarSize Buffer
	memset(vsb, 0x88, sizeof(vsb));

	size_t osz; // Out SiZe
	if (ls_varsize_get_bytes(vsb, &osz, vsv).success) {
		ls_vs_value_t ov;
		if (ls_varsize_get_value(&ov, vsb, sizeof(vsb)).success) {
			ls_logf("buffer..........: 0x%"PRIXPTR, (uintptr_t)vsb);
			ls_logf("buffer_size.....: %"PRIuMAX, (uintmax_t)sizeof(vsb));
			ls_logf("original value..: %"PRIuMAX, vsv);
			ls_logf("new value.......: %"PRIuMAX, ov);
			ls_logf("value size......: %"PRIuMAX, (uintmax_t)sizeof(vsv));
			ls_logf("bytes used......: %"PRIuPTR, osz);

			double ratio = (((double)osz / ((double)sizeof(vsv))) * 100.0D);
			ls_logf("ratio...........: \033[3%s;1m%.2lf%%\033[0m", ((ratio < 100.0D) ? "2" : ((ratio == 100.0D) ? "3" : "1")), ratio);
			puts("");
			ls_vmemdump(vsb, sizeof(vsb), "buffer contents");
			return 0;
		} else {
			puts("error");
			return 2;
		}
	} else {
		puts("error");
		return 1;
	}

	puts("TODO: some reasonable testing");
	return 0;
}


/*#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <libserum/crypto/xxtea.h>
#include <libserum/crypto/padding/iso9797.h>
#include <libserum/debug/memdump.h>
#include <libserum/core/time.h>
#include <libserum/core/bits.h>
#include <libserum/core/varsize.h>
#include <libserum/crypto/prng/device.h>
#include <libserum/crypto/prng/isaac.h>
#include <libserum/core/memory.h>

#include <inttypes.h>


static uint64_t testexpr(const char *desc, char expr_true, char expr_false, uint64_t expr) {
	printf("\033[3%c;1m%c\033[0m %s\n", ((expr) ? '2' : '1'), ((expr) ? expr_true : expr_false), desc);
	return expr;
}


int main(int argc, char *argv[], char *env[]) {
	uint64_t value = 1234456789;
	uint8_t data[10];
	size_t size;
	ls_memdump(data, sizeof(data));
	if (ls_varsize_get_bytes(data, &size, value).success) {
		uint64_t value2 = 0;
		if (ls_varsize_get_value(&value2, data).success) {
			ls_memdump(data, sizeof(data));
			printf("org = %"PRIu64"\nnew = %"PRIu64"\n", value, value2);
		}
	}
	return 0;

#if (!LS_WINDOWS)
	void *next = &&lbl_rot;
#endif

lbl_rot: {
#if (!LS_WINDOWS)
		next = &&lbl_bitmask;
#endif

		uint32_t i, r;
		i = 0b00001101011101010010100010101110;

		r = 0b01011101010010100010101110000011;
		testexpr("LS_ROTL32", 'y', 'n', (LS_ROTL32(i, 6) == r));

		r = 0b10111000001101011101010010100010;
		testexpr("LS_ROTR32", 'y', 'n', (LS_ROTR32(i, 6) == r));
	}

lbl_bitmask: {
#if (!LS_WINDOWS)
		next = &&lbl_time;
#endif

		testexpr("BITMASK", 'y', 'n', ((BITMASK(4) == 0x0F) && (BITMASK(8) == 0xFF) && (BITMASK(16) == 0xFFFF) && (BITMASK(32) == 0xFFFFFFFF) && (BITMASK(48) == 0xFFFFFFFFFFFF)));
	}

lbl_time: {
#if (!LS_WINDOWS)
		//next = &&lbl_iso9797;
#endif

		testexpr("ls_rdtsc", 'y', 'n', (ls_rdtsc() != 0));
		testexpr("ls_nanos", 'y', 'n', (ls_nanos() != 0));
	}

lbl_iso9797: {
#if (!LS_WINDOWS)
		next = &&lbl_prng_device;
#endif

		size_t size;

		uint8_t stackalloc(input, 55);
		uint8_t stackalloc(buffer, (size = ls_pad_iso9797_size(16, stacksizeof(input))));
		uint8_t stackalloc(zeroed, stacksizeof(buffer) - stacksizeof(input));

		memset(input, 0x69, stacksizeof(input));
		memset(buffer, 0xBA, size);
		memset(zeroed, 0, stacksizeof(zeroed));

		ls_memdump(input, stacksizeof(input));
		ls_memdump(buffer, size);

		ls_pad_iso9797_ex(buffer, input, stacksizeof(input), size);

		ls_memdump(buffer, size);

		testexpr("ls_pad_iso9797_ex", 'y', 'n', (memcmp(input, buffer, stacksizeof(input)) == 0) && (buffer[stacksizeof(input)] == 0x80) && (memcmp(buffer + stacksizeof(input) + 1, zeroed, stacksizeof(buffer) - stacksizeof(input) - 1) == 0));

		size_t offset = 0;
		testexpr("ls_pad_iso9797_offset", 'y', 'n', (ls_pad_iso9797_offset(&offset, buffer, size).success && (offset == stacksizeof(input))));

		stackfree(buffer);
		stackfree(zeroed);
	}

lbl_prng_device: {
#if (!LS_WINDOWS)
		next = &&lbl_prng_isaac;
#endif

		ls_prng_device_t device;
		if (testexpr("ls_prng_device_sys", 'y', 'n', ls_prng_device_sys(&device, 512, (DEV_HARDWARE | DEV_FORCE_UNLIMITED)).success)) {
			uint8_t pad[8];
			memset(pad, 0xBA, sizeof(pad));

			uint8_t buffer[616];
			memset(buffer, 0xBA, sizeof(buffer));

			uint8_t *ptr = (buffer + 8);
			size_t size = 600;
			testexpr("ls_prng_device_generate", 'y', 'n', (ls_prng_device_generate(&device, ptr, size).success && (memcmp(buffer, pad, 8) == 0) && (memcmp(buffer + 608, pad, 8) == 0)));
			testexpr("ls_prng_device_clear", 'y', 'n', ls_prng_device_clear(&device).success);
		}
	}

lbl_prng_isaac: {
#if (!LS_WINDOWS)
		next = &&lbl_xxtea_simple;
#endif

		testexpr("ls_prng_isaac_test", 'y', 'n', ls_prng_isaac_test().success);
	}

lbl_xxtea_simple: {
#if (!LS_WINDOWS)
		next = &&lbl_end;
#endif

		uint32_t key  [4] = { 0xBABAB0B0, 0xCACAC0C0, 0xDADADEDE, 0xBACAD0FE };
		uint32_t input[5] = { 0xDEADBEEF, 0xF00BAAAA, 0xCAFEBABE, 0xBEEFBABE, 0xDEFEC8ED };
		uint8_t buffer[sizeof(input)];

		if (!ls_xxtea_encrypt(buffer, input, sizeof(buffer), key).success) {
			puts("ls_xxtea_encrypt: e");
#if (!LS_WINDOWS)
			goto *next;
#endif
		}
		testexpr("ls_xxtea_encrypt", 'y', 'n', (memcmp(buffer, input, sizeof(input)) != 0));

		if (!ls_xxtea_decrypt(NULL, buffer, sizeof(buffer), key).success) {
			puts("ls_xxtea_decrypt: e");
#if (!LS_WINDOWS)
			goto *next;
#endif
		}
		testexpr("ls_xxtea_decrypt", 'y', 'n', !memcmp(buffer, input, sizeof(input)));
	}

lbl_end:
	fgetc(stdin);
	return 0;
}
*/
