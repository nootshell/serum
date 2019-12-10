#include "./memory.h"

#include <stdint.h>
#include <stdlib.h>




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
