#ifndef __SERUM_CORE_SANITY_COMPILETIME_H
#define __SERUM_CORE_SANITY_COMPILETIME_H

#ifndef __SERUM_CORE_SANITY_H
#	error Expecting to be included from serum/core/sanity.h, which was not detected.
#endif




SERUM_COMPILER_PUSH
SERUM_COMPILER_PEDANTIC




SERUM_ASSERT(assert_sanity, (sizeof(unsigned int) != sizeof(unsigned char)));




SERUM_COMPILER_POP




#endif
