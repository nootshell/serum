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

#include <libserum/crypto/key_exchange/curve25519.h>
#include <libserum/crypto/prng/device.h>


#define STATIC								0

int main(int argc, char *argv[], char *env[]) {
#if (!STATIC)
	ls_device_t device;
	ls_device_sys(&device, 16, DEV_URANDOM);
#endif

	ls_curve25519_key_t basepoint;
	memset(basepoint, 0xCA, sizeof(basepoint));

	ls_curve25519_key_t private_key_alice;
#if (STATIC)
	memset(private_key_alice, 0xBA, sizeof(private_key_alice));
#else
	ls_device_generate(&device, private_key_alice, sizeof(private_key_alice));
#endif

	ls_curve25519_key_t private_key_bob;
#if (STATIC)
	memset(private_key_bob, 0x69, sizeof(private_key_bob));
#else
	ls_device_generate(&device, private_key_bob, sizeof(private_key_bob));
#endif

	ls_curve25519_t curve_alice, curve_bob;

	if (!ls_curve25519_init_ex(&curve_alice, private_key_alice, basepoint).success) {
		return 1;
	}
	if (!ls_curve25519_init_ex(&curve_bob, private_key_bob, basepoint).success) {
		return 2;
	}

	if (!ls_curve25519_generate_shared(&curve_alice, ls_curve25519_get_public(&curve_bob)).success) {
		return 3;
	}
	if (!ls_curve25519_generate_shared(&curve_bob, ls_curve25519_get_public(&curve_alice)).success) {
		return 4;
	}

	if (!ls_memdiff(ls_curve25519_get_shared(&curve_alice), ls_curve25519_get_shared(&curve_bob), sizeof(ls_curve25519_key_t))) {
		puts(LS_ANSI_ESCAPE LS_ANSI_FG_GREEN LS_ANSI_OPT LS_ANSI_BRIGHT LS_ANSI_TERMINATE "passed" LS_ANSI_RESET);
	} else {
		puts(LS_ANSI_ESCAPE LS_ANSI_FG_RED LS_ANSI_OPT LS_ANSI_BRIGHT LS_ANSI_TERMINATE "failed" LS_ANSI_RESET);
	}

	if (!ls_curve25519_clear(&curve_alice).success) {
		return 5;
	}
	if (!ls_curve25519_clear(&curve_bob).success) {
		return 6;
	}

	return 0;
}
