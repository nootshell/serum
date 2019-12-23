#ifndef __SERUM_CORE_SANITY_H
#define __SERUM_CORE_SANITY_H




#include <errno.h>




#define SERUM_OK							0
#define SERUM_ERRNO							1	/*!< \hideinitializer \brief Returned when errno describes the error perfectly fine. */
#define SERUM_NULLPTR						2	/*!< \hideinitializer \brief Nullpointer encountered where NULL was not expected. */




#define SERUM_RETERRNOIFEXPR(__expr, __ret, __errno)	\
	if ((__expr)) {										\
		errno = (__errno);								\
		return (__ret);									\
	}

#define SERUM_CHECK_NULLPTR(__ptr)			SERUM_RETERRNOIFEXPR(((__ptr) == NULL), SERUM_NULLPTR, ENOMSG)




#endif
