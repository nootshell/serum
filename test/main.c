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
 **    -
 **
 */

#define FILE_PATH							"main.c"
#define DEBUG								1

#include <stdio.h>
#include <string.h>

#include <libserum/core/info.h>
#include <libserum/debug/memdump.h>
#include <libserum/crypto/symmetric/salsa20.h>


void
static __exit(int code) {
	printf("return %i\n", code);
	fgetc(stdin);
	exit(code);
}


int main(int argc, char *argv[], char *env[]) {
	puts(ls_info_compilation_environment());
	puts(ls_info_compilation_options());
	puts("");

	uint8_t key[16];
	memset(key, 0xBA, sizeof(key));


	uint8_t data[130];
	memset(data, 0x69, sizeof(data));

	uint8_t encrypted[176], *ct = encrypted + 16;
	memset(encrypted, 0x55, sizeof(encrypted));

	uint8_t decrypted[sizeof(encrypted)], *pt = decrypted + 16;
	memset(decrypted, 0x55, sizeof(decrypted));


	ls_vmemdump(key, sizeof(key), "key:");
	ls_vmemdump(data, sizeof(data), "data:");
	ls_vmemdump(encrypted, sizeof(encrypted), "encrypt buffer:");
	ls_vmemdump(decrypted, sizeof(decrypted), "decrypt buffer:");


	ls_salsa20_t salsa_encrypt, salsa_decrypt;
	
	if (!ls_salsa20_init(&salsa_encrypt, key, sizeof(key)).success) {
		__exit(1);
	}
	if (!ls_salsa20_init(&salsa_decrypt, key, sizeof(key)).success) {
		__exit(2);
	}

	memcpy(ct, data, sizeof(data));
	ls_vmemdump(encrypted, sizeof(encrypted), "encrypt:");
	if (!ls_salsa20_encrypt(&salsa_encrypt, ct, sizeof(data)).success) {
		__exit(3);
	}
	ls_vmemdump(encrypted, sizeof(encrypted), "encrypted:");

	memcpy(pt, ct, sizeof(data));
	ls_vmemdump(decrypted, sizeof(decrypted), "decrypt:");
	if (!ls_salsa20_decrypt(&salsa_decrypt, pt, sizeof(data)).success) {
		__exit(4);
	}
	ls_vmemdump(decrypted, sizeof(decrypted), "decrypted:");

	if (!ls_salsa20_clear(&salsa_encrypt).success) {
		__exit(5);
	}
	if (!ls_salsa20_clear(&salsa_decrypt).success) {
		__exit(6);
	}

	__exit(0);
}
