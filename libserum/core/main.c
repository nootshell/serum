/******************************************************************************
**                                                                           **
**   The MIT License                                                         **
**                                                                           **
**   Copyright 2017-2018 icecubetray                                         **
**                                                                           **
**   Permission is hereby granted, free of charge, to any person             **
**   obtaining a copy of this software and associated documentation files    **
**   (the "Software"), to deal in the Software without restriction,          **
**   including without limitation the rights to use, copy, modify, merge,    **
**   publish, distribute, sublicense, and/or sell copies of the Software,    **
**   and to permit persons to whom the Software is furnished to do so,       **
**   subject to the following conditions:                                    **
**                                                                           **
**   The above copyright notice and this permission notice shall be          **
**   included in all copies or substantial portions of the Software.         **
**                                                                           **
**   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,         **
**   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF      **
**   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  **
**   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    **
**   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    **
**   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       **
**   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  **
**                                                                           **
******************************************************************************/


#include "./setup.h"

#include "../io/log.h"
#include "../runtime/concurrency/mutex.h"
#include "../runtime/concurrency/state.h"
#include "../runtime/concurrency/thread.h"
#include "../runtime/event.h"

#include <stdio.h>
#include <inttypes.h>




#ifndef KERNEL
#	if (LS_LINUX)
#		define KERNEL						"Linux"
#	elif (LS_WINDOWS)
#		define KERNEL						"NT"
#	else
#		define KERNEL						"Unknown"
#	endif
#endif

#ifndef KERNEL_ARCH
#	define KERNEL_ARCH						""
#endif

#if (LS_GCC)
#	define COMPILER							"GCC"
#elif (LS_LLVM)
#	define COMPILER							"LLVM"
#elif (LS_MINGW)
#	define COMPILER							"MinGW"
#elif (LS_MSC)
#	define COMPILER							"MSC"
#else
#	define COMPILER							"Unknown"
#endif

#if (LS_X86)
#	define ARCH								"x86"
#elif (LS_X64)
#	define ARCH								"x64"
#elif (LS_ARM)
#	define ARCH								"ARM"
#else
#	define ARCH								"Unknown"
#endif

#define PRINT_SZ(type)						printf("\t%5"PRIuPTR" = %s\n", sizeof(type), LS_MKSTR(type));




#define CORE_FILEID							"libserum (" LS_VERSION ", \"" LS_CODENAME "\") built for " ARCH " with " COMPILER " on " KERNEL " " KERNEL_ARCH " at " TIMESTAMP
FILEID_PLAIN(___COREID___, ">>> " CORE_FILEID " <<<");




#if (defined(ELF_INTERPRETER) && LS_LINUX)




FILEID("ELF entrypoint for debugging/self-testing purposes.");




const char LS_ATTR_USED __LS_ATTR(section(".interp")) interp[] = ELF_INTERPRETER;

void
LS_ATTR_NORETURN __libserum_main(const void *const d_ptr, const int argc, const char *const *const argv, const char *const *const env) {
	puts(CORE_FILEID);

	puts("");
	puts("\tBytes   Type");
	PRINT_SZ(uint8_t);
	PRINT_SZ(uint16_t);
	PRINT_SZ(uint32_t);
	PRINT_SZ(uint64_t);
	PRINT_SZ(void*);

	puts("");
	puts("\tBytes   Structure");
	PRINT_SZ(ls_log_t);
	PRINT_SZ(ls_mutex_t);
	PRINT_SZ(ls_state_t);
	PRINT_SZ(ls_thread_t);
	PRINT_SZ(ls_event_t);

	// This whole function is one big HACK, but the arguments are even more of a HACK
	// which I may or may not drop later on depending on how big of a mess it'll be.
	//int i;
	//printf("Got %u arguments:\n", argc);
	//for (i = 0; i < argc; ++i) {
	//	printf("\t%u: %p\n", i, argv[i]);
	//}

	exit(0);
}




#endif
