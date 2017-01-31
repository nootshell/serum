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

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <libserum/crypto/xxtea.h>
#include <libserum/crypto/padding/iso9797.h>
#include <libserum/debug/memdump.h>
#include <libserum/core/time.h>
#include <libserum/core/bits.h>
#include <libserum/crypto/prng/device.h>


static void testexpr(const char *desc, char expr_true, char expr_false, uint64_t expr) {
	printf("\033[3%c;1m%c\033[0m %s\n", ((expr) ? '2' : '1'), ((expr) ? expr_true : expr_false), desc);
}


int main(int argc, char *argv[], char *env[]) {
	void *next = &&lbl_rot;

lbl_rot: {
		next = &&lbl_time;

		uint32_t i, r;
		i = 0b00001101011101010010100010101110;

		r = 0b01011101010010100010101110000011;
		testexpr("LS_ROTL32", 'y', 'n', (LS_ROTL32(i, 6) == r));

		r = 0b10111000001101011101010010100010;
		testexpr("LS_ROTR32", 'y', 'n', (LS_ROTR32(i, 6) == r));
	}

lbl_time: {
		next = &&lbl_iso9797;
		testexpr("ls_rdtsc", 'y', 'n', (ls_rdtsc() != 0));
		testexpr("ls_nanos", 'y', 'n', (ls_nanos() != 0));
	}

lbl_iso9797: {
		next = &&lbl_prng_device;

		size_t size;
		uint8_t input[55], buffer[size = ls_pad_iso9797_size(16, sizeof(input))], zeroed[sizeof(buffer) - sizeof(input)];
		memset(input, 0x69, sizeof(input));
		memset(buffer, 0xBA, size);
		memset(zeroed, 0, sizeof(zeroed));

		ls_pad_iso9797_ex(buffer, input, sizeof(input), size);

		testexpr("ls_pad_iso9797_ex", 'y', 'n', (memcmp(input, buffer, sizeof(input)) == 0) && (buffer[sizeof(input)] == 0x80) && (memcmp(buffer + sizeof(input) + 1, zeroed, sizeof(buffer) - sizeof(input) - 1) == 0));
	}

lbl_prng_device: {
		next = &&lbl_xxtea_simple;

		ls_prng_device_t device;

		if (ls_prng_device_sys(&device, 512, (DEV_HARDWARE | DEV_FORCE_UNLIMITED)).success) {
			uint8_t buffer[616];
			memset(buffer, 0xBA, sizeof(buffer));
			uint8_t *ptr = (buffer + 8);
			size_t size = 600;
			if (ls_prng_device_generate(&device, ptr, size).success) {
				ls_memdump_ex(buffer, sizeof(buffer), 8, 4);
			}
			if (!ls_prng_device_clear(&device).success) {
				puts("ls_prng_device_clear: cleanup failure");
			}
		}
	}

lbl_xxtea_simple: {
		next = &&lbl_end;

		uint32_t key  [4] = { 0xBABAB0B0, 0xCACAC0C0, 0xDADADEDE, 0xBACAD0FE };
		uint32_t input[5] = { 0xDEADBEEF, 0xF00BAAAA, 0xCAFEBABE, 0xBEEFBABE, 0xDEFEC8ED };
		uint8_t buffer[sizeof(input)];

		if (!ls_xxtea_encrypt(buffer, input, sizeof(buffer), key).success) {
			puts("ls_xxtea_encrypt: e");
			goto *next;
		}
		testexpr("ls_xxtea_encrypt", 'y', 'n', (memcmp(buffer, input, sizeof(input)) != 0));

		if (!ls_xxtea_decrypt(NULL, buffer, sizeof(buffer), key).success) {
			puts("ls_xxtea_decrypt: e");
			goto *next;
		}
		testexpr("ls_xxtea_decrypt", 'y', 'n', !memcmp(buffer, input, sizeof(input)));
	}

lbl_end:
	return 0;
}
