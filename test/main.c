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

#include <libserum/core/stdincl.h>
#include <libserum/debug/log.h>
#include <libserum/debug/memdump.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <libserum/crypto/symmetric/rijndael.h>
#include <libserum/crypto/symmetric/xxtea.h>



int main(int argc, char *argv[], char *env[]) {
	uint8_t key[32];
	memset(key, 'a', sizeof(key));

	uint32_t data[4];
	data[0] = 0xDEADBEEF;
	data[1] = 0xCAFEBABE;
	data[2] = 0xDEFEC8ED;
	data[3] = 0xBEEFDEAD;

	uint32_t buffer[4] = { data[0], data[1], data[2], data[3] };



	ls_rijndael_t ctx;
	memset(&ctx, 0x88, sizeof(ctx));

	if (!ls_rijndael_init(&ctx, key, sizeof(key)).success) {
		return 1;
	}
	ls_vmemdump(ctx.key_encrypt->data, ctx.key_encrypt->size, "Encryption key:"); puts("");
	ls_vmemdump(ctx.key_decrypt->data, ctx.key_decrypt->size, "Decryption key:"); puts("");

	ls_vmemdump(buffer, sizeof(buffer), "Buffer, original:");

	if (!ls_rijndael_encrypt_block(&ctx, buffer).success) {
		return 2;
	}
	ls_vmemdump(buffer, sizeof(buffer), "Buffer, encrypted:");

	if (!ls_rijndael_decrypt_block(&ctx, buffer).success) {
		return 3;
	}
	ls_vmemdump(buffer, sizeof(buffer), "Buffer, decrypted:");

	if (!ls_rijndael_clear(&ctx).success) {
		return 4;
	}

	/*ls_xxtea_t ctx;
	memset(&ctx, 0x88, sizeof(ctx));

	if (!ls_xxtea_init(&ctx, key).success) {
		return 1;
	}
	ls_vmemdump(ctx.key->data, ctx.key->size, "Key:"); puts("");

	const char plaintext[8] = "testtext";
	uint32_t xd[2];
	memcpy(xd, plaintext, sizeof(plaintext));

	ls_vmemdump(xd, sizeof(xd), "Buffer, original:"); puts("");

	if (!ls_xxtea_encrypt_block(&ctx, xd, (sizeof(xd) / sizeof(*xd))).success) {
		return 2;
	}
	ls_vmemdump(xd, sizeof(xd), "Buffer, encrypted:"); puts("");

	if (!ls_xxtea_decrypt_block(&ctx, xd, (sizeof(xd) / sizeof(*xd))).success) {
		return 3;
	}
	ls_vmemdump(xd, sizeof(xd), "Buffer, decrypted:"); puts("");

	if (!ls_xxtea_clear(&ctx).success) {
		return 4;
	}*/

	return 0;
}
