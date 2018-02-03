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

#include <stdio.h>



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



FILEID_PLAIN(___COREID___, ">> libserum (" LS_VERSION ", \"" LS_CODENAME "\") built for " ARCH " with " COMPILER " on " KERNEL " " KERNEL_ARCH " at " TIMESTAMP " <<");



#if (defined(ELF_INTERPRETER) && LS_LINUX)



FILEID("ELF entrypoint for self-testing.");

const char LS_ATTR_USED __attribute__((section(".interp"))) interp[] = ELF_INTERPRETER;



void LS_ATTR_NORETURN __libserum_main(void *dummy, int argc, char **argv, char **env) {
	puts("hi");
	printf("dummy at %p\n", dummy);
	printf("got %u args at %p\n", argc, argv);
	exit(0);
}
__attribute__((section(".init_array"), aligned (sizeof (void *)))) typeof(__libserum_main) *init = &__libserum_main;



#endif
