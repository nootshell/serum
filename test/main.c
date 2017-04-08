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

#include <libserum/core/time.h>
#include <libserum/crypto/hashing/md5.h>
#include <libserum/crypto/hashing/sha2.h>
#include <libserum/crypto/hmac/hmac-md5.h>
#include <libserum/crypto/hmac/hmac-sha2.h>
#include <libserum/runtime/thread.h>

uint16_t thrfunc(ls_thread_t *thread) {
	do {
		if (!ls_thread_check_suspended(thread).success) {
			puts("Suspend check failed.");
			return LS_RESULT_CODE_EARLY_EXIT;
		}

		if (!ls_thread_check_exit(thread).success) {
			puts("Exiting.");
			return LS_RESULT_CODE_ABORTED;
		}

		printf("Thread %"PRIuPTR" is happily running.\n", thread->thread);
		ls_sleep_millis(500);
	} while (1);
}

signed int tries = 5;
ls_bool stpfunc(ls_thread_t *thread) {
	return (--tries <= 0);
}

int main(int argc, char *argv[], char *env[]) {
	/*if (argc > 2) {
		char *key = argv[1];
		size_t keylen = strlen(key);
		char *data = argv[2];
		size_t datalen = strlen(data);

		ls_md5_digest_t md5;
		ls_sha2_224_digest_t sha2_224;
		ls_sha2_256_digest_t sha2_256;
		ls_sha2_384_digest_t sha2_384;
		ls_sha2_512_digest_t sha2_512;

		if (!ls_hmac_md5(data, datalen, key, keylen, md5).success) {
			return 1;
		}
		if (!ls_hmac_sha2_224(data, datalen, key, keylen, sha2_224).success) {
			return 2;
		}
		if (!ls_hmac_sha2_256(data, datalen, key, keylen, sha2_256).success) {
			return 3;
		}
		if (!ls_hmac_sha2_384(data, datalen, key, keylen, sha2_384).success) {
			return 4;
		}
		if (!ls_hmac_sha2_512(data, datalen, key, keylen, sha2_512).success) {
			return 5;
		}

		ls_vmemdump(md5, sizeof(md5), "HMAC-MD5:");
		ls_vmemdump(sha2_224, sizeof(sha2_224), "\nHMAC-SHA2-224:");
		ls_vmemdump(sha2_256, sizeof(sha2_256), "\nHMAC-SHA2-256:");
		ls_vmemdump(sha2_384, sizeof(sha2_384), "\nHMAC-SHA2-384:");
		ls_vmemdump(sha2_512, sizeof(sha2_512), "\nHMAC-SHA2-512:");

		return 0;
	} else {
		puts("HMAC test utility for HMAC-MD5 and HMAC-SHA2 (224, 256, 384, 512).");
		printf("Usage: %s <key> <data>\n", argv[0]);
	}
	return 64;*/

	ls_thread_t thread;
	if (!ls_thread_init(&thread, thrfunc).success) {
		return 1;
	}
	if (!ls_thread_set_stop_handler(&thread, stpfunc).success) {
		return 2;
	}
	if (!ls_thread_start(&thread).success) {
		return 3;
	}

	while (tries) {
		fgetc(stdin);

		ls_result_t result = ls_thread_stop(&thread);
		if (!result.success) {
			if (result.code != LS_RESULT_CODE_ABORTED) {
				printf("%04X %u\n", result.code, result.param);
				return 4;
			} else {
				if (tries == 4) {
					if (!ls_thread_suspend(&thread).success) {
						return 6;
					}
					puts("Thread suspended.");
				} else if (tries == 3) {
					if (!ls_thread_resume(&thread).success) {
						return 7;
					}
					puts("Thread resumed.");
				} else {
					printf("Stop aborted: %i\n", tries);
				}
			}
		} else {
			puts("Stop succeeded.");
		}
	}

	if (!ls_thread_clear(&thread).success) {
		return 5;
	}

	return 0;
}
