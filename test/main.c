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

#define FILE_PATH							"main.c"
#define DEBUG								1

#include <stdio.h>
#include <string.h>

#include <libserum/core/time.h>
#include <libserum/core/memory.h>
#include <libserum/debug/memdump.h>

#include <libserum/crypto/kdf/pbkdf2-sha2.h>
#include <libserum/crypto/kdf/scrypt.h>


#define STATIC								1


const uint64_t n = 7500;

void bench(double *s, double *g, void *cmp1, void *cmp2, size_t size) {
	uint64_t
		s_t[n],
		g_t[n];

	uint64_t start;

	uint64_t i;
	for (i = 0; i < n; ++i) {
		start = ls_nanos();
		ls_bool r2 = ls_memequ(cmp1, cmp2, size);
		s_t[i] = (ls_nanos() - start);
		//ls_log(LS_LOG_INFO, "ls_memequ took %luns and returned %lu", s_t[i], r2);

		start = ls_nanos();
		int r1 = memcmp(cmp1, cmp2, size);
		g_t[i] = (ls_nanos() - start);
		//ls_log(LS_LOG_INFO, "   memcmp took %luns and returned %i", g_t[i], r1);
	}

	uint64_t s_total = 0, g_total = 0;
	for (i = n; i--;) {
		s_total += s_t[i];
		g_total += g_t[i];
	}

	*s = (s_total / (double)n);
	*g = (g_total / (double)n);
}

int main(int argc, char *argv[], char *env[]) {
	if (argc < 4) {
		uint8_t kutjebef[250], asdfasdf[sizeof(kutjebef)];
		ls_memory_destroy(kutjebef, sizeof(kutjebef));
		ls_memory_destroy(asdfasdf, sizeof(asdfasdf));

		asdfasdf[sizeof(kutjebef) - 1] = 1;

		double s_total = 0, g_total = 0, s, g;
		uint64_t i;
		for (i = (n << 1); i--;) {
			bench(&s, &g, kutjebef, asdfasdf, sizeof(kutjebef));
			s_total += s;
			g_total += g;
		}

		ls_log(LS_LOG_INFO, "%s wins. mean: s=%.3lf g=%.3lf", ((s_total < g_total) ? "serum" : "c std"), (s_total / (n << 1)), (g_total / (n << 1)));

		//printf("Usage: %s <password> <salt> <rounds> <keylen>\n", argv[0]);
		return 1;
	} else {
		char *pass = argv[1];
		size_t pass_size = strlen(pass);
		char *salt = argv[2];
		size_t salt_size = strlen(salt);
		uintmax_t rounds = strtoumax(argv[3], NULL, 10);
		uintmax_t r = strtoumax(argv[4], NULL, 10);
		uintmax_t p = strtoumax(argv[5], NULL, 10);
		uintmax_t out_size = strtoumax(argv[6], NULL, 10);

		ls_memory_area_t out_area;
		ls_memory_area_init(&out_area, out_size);
		uint8_t *out = out_area.data;
		if (!ls_scrypt(out, out_size, pass, pass_size, salt, salt_size, (uint32_t)rounds, (uint32_t)r, (uint32_t)p).success) {
			return 1;
		}

		printf("scrypt(\"%s\", \"%s\", %" PRIuMAX ", %" PRIuMAX ", %" PRIuMAX ", %" PRIuMAX ") =\n", pass, salt, rounds, r, p, out_size);
		out -= 4;
		out_size += 8;
		ls_memdump(out, out_size);

		ls_memory_area_clear(&out_area);
		ls_memdump(out, out_size);
	}

	return 0;
}
