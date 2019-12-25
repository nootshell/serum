#include "./memory.h"

#include <stdint.h>
#include <stdlib.h>

#include <stdio.h>




void
serum_erase(void *const target, const size_t size) {
	if (target == NULL || size == 0) {
		return;
	}

#if (SERUM_DEBUG_MEMORY)
#	define VALUE	0xEE
#else
#	define VALUE	0x00
#endif

	register size_t n;
	for (n = size; n--;) {
		*(volatile char *const)((uintptr_t)target + n) = VALUE;
	}

	*(volatile char *const)target = *(volatile char *const)target;

#undef VALUE
}




void
serum_memdump(const void *const src, const size_t size) {
	const unsigned char *s8 = src;
	size_t register i, c;

	const unsigned int columns = 8;

	for (i = c = 0; i < size; ++i) {
		printf("%02X ", *s8++);

		if (++c == columns) {
			c = 0;
			puts("");
		}
	}

	if (c != 0) {
		puts("");
	}
}
