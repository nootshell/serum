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

#include <libserum/core/memory.h>
#include <libserum/debug/memdump.h>

#include <libserum/crypto/kdf/pbkdf2-sha2.h>
#include <libserum/crypto/hmac/hmac-sha2.h>


#define STATIC								1


int main(int argc, char *argv[], char *env[]) {
	if (argc < 4) {
		printf("Usage: %s <password> <salt> <rounds> <keylen>\n", argv[0]);
		return 1;
	} else {
		size_t pwsz = strlen(argv[1]);
		size_t stsz = strlen(argv[2]);
		uintmax_t rounds = strtoumax(argv[3], NULL, 10);
		uintmax_t keysz = strtoumax(argv[4], NULL, 10);

		uint8_t stackalloc(key, keysz);
		ls_pbkdf2_sha2_256(key, keysz, argv[1], pwsz, argv[2], stsz, rounds);

		ls_vmemdump(argv[1], pwsz, "Password");
		ls_vmemdump(argv[2], stsz, "Salt");
		ls_vmemdump(key, keysz, "Output");
	}

	return 0;
}
