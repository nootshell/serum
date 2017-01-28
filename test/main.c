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


int main(int argc, char *argv[], char *env[]) {
	void *next = &&lbl_iso9797;

lbl_iso9797:
	{
		next = &&lbl_xxtea_simple;

		size_t size;
        uint8_t input[55], buffer[size = ls_pad_iso9797_size(16, sizeof(input))];
        memset(input, 0x69, sizeof(input));
        memset(buffer, 0xBA, size);

        ls_memdump(input, sizeof(input));
        ls_memdump(buffer, sizeof(buffer));

		ls_pad_iso9797(buffer, input, sizeof(input), size);

		ls_memdump(input, sizeof(input));
		ls_memdump(buffer, sizeof(buffer));
	}

lbl_xxtea_simple:
	{
		next = &&lbl_end;

		puts("xxtea (simple, 1)");

		uint32_t key  [4] = { 0xBABAB0B0, 0xCACAC0C0, 0xDADADEDE, 0xBACAD0FE };
		uint32_t input[5] = { 0xDEADBEEF, 0xF00BAAAA, 0xCAFEBABE, 0xBEEFBABE, 0xDEFEC8ED };
		uint8_t buffer[sizeof(input)];

		ls_memdump_ex(key, sizeof(key), 4, 4);

		if (!ls_xxtea_encrypt(buffer, input, sizeof(buffer), key).success) {
			puts("encrypt: e");
			goto *next;
		}
		printf("encrypt: %c\n", (memcmp(buffer, input, sizeof(input)) ? 'y' : 'n'));

		if (!ls_xxtea_decrypt(NULL, buffer, sizeof(buffer), key).success) {
			puts("decrypt: e");
			goto *next;
		}
		printf("decrypt: %c\n", (memcmp(buffer, input, sizeof(input)) ? 'n' : 'y'));
	}

lbl_end:
	return 0;
}
