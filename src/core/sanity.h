#ifndef __SERUM_CORE_SANITY_H
#define __SERUM_CORE_SANITY_H




#include <errno.h>




#define SERUM_OK							0
#define SERUM_ESYSTEM						1	/*!< \hideinitializer \brief Returned when a call to a system function failed, errno should be untouched. */
#define SERUM_ENULLPTR						2	/*!< \hideinitializer \brief Nullpointer encountered where NULL was not expected. */
#define SERUM_ERANGE						3	/*!< \hideinitializer \brief Value is out of the expected range. */
#define SERUM_EALGORITHM					4	/*!< \hideinitializer \brief Algorithm error. */




#define SERUM_ASSERT(__name, __expr)		extern int __name[(!!(__expr))]




#define SERUM_SANITY_AREA(__x__)			__x__


#define SERUM_RETERRNOIFEXPR(__expr, __ret, __errno)		\
	if ((__expr)) {											\
		if ((__errno) != 0 && (__ret) != SERUM_ESYSTEM) {	\
			errno = (__errno);								\
		}													\
		return (__ret);										\
	}

#define SERUM_CHECK_NULLPTR(__ptr)			SERUM_RETERRNOIFEXPR(((__ptr) == NULL), SERUM_ENULLPTR, 0)
#define SERUM_CHECK_RANGE(__val, _lb, _ub)	SERUM_RETERRNOIFEXPR(( ( (__val) < (_lb) ) || ( (__val) > (_ub) ) ), SERUM_ERANGE, 0)




#include "./sanity-compiletime.h"




#endif
