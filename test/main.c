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

#include <libserum/crypto/hmac/hmac-sha2.h>


int main(int argc, char *argv[], char *env[]) {
	char key[] = "Jefe";
	char data[] = "what do ya want for nothing?";

	ls_sha2_224_digest_t d224;
	ls_sha2_256_digest_t d256;
	ls_sha2_384_digest_t d384;
	ls_sha2_512_digest_t d512;

	ls_hmac_sha2_224(data, strlen(data), key, strlen(key), d224);
	ls_hmac_sha2_256(data, strlen(data), key, strlen(key), d256);
	ls_hmac_sha2_384(data, strlen(data), key, strlen(key), d384);
	ls_hmac_sha2_512(data, strlen(data), key, strlen(key), d512);

	ls_vmemdump(d224, sizeof(d224), "HMAC-SHA2-224");
	ls_vmemdump(d256, sizeof(d256), "HMAC-SHA2-256");
	ls_vmemdump(d384, sizeof(d384), "HMAC-SHA2-384");
	ls_vmemdump(d512, sizeof(d512), "HMAC-SHA2-512");

	return 0;
}
