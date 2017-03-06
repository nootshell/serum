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

#ifndef __LS_NETWORKING_SOCKET_H
#define __LS_NETWORKING_SOCKET_H


#include "../core/stdincl.h"


#define LS_INVALID_SOCKET					(((uint32_t)0)-1)


#define LS_SOCKET_EXITED					BIT_1	// Read-only, TODO
#define LS_SOCKET_ACCEPTED					BIT_2	// Read-only, TODO
#define LS_SOCKET_REFUSED					BIT_3	// Read-only, TODO
#define LS_SOCKET_SERVER					BIT_4

#define LS_SOCKET_REUSEADDR					BIT_5	// TODO
#define LS_SOCKET_REUSEPORT					BIT_6	// TODO
#define LS_SOCKET_REUSE						(LS_SOCKET_REUSEADDR | LS_SOCKET_REUSEPORT)

#define LS_SOCKET_ASYNC						BIT_7	// TODO
#define LS_SOCKET_ASYNC_CHILDREN			BIT_8	// TODO

#define LS_SOCKET_UDP						BIT_9

#define LS_SOCKET_CRYPTO					BIT_10	// TODO


typedef struct ls_socket {
	struct addrinfo *addrinfo;
	uint32_t fd;
	uint32_t flags;
} ls_socket_t;


#ifdef __cplusplus
extern "C" {
#endif

	ls_result_t ls_socket_init_ex(ls_socket_t *const ctx, char const *node, uint32_t const flags);
	ls_result_t ls_socket_init(ls_socket_t *const ctx, char const *node);
	ls_result_t ls_socket_clear(ls_socket_t *const ctx);

#ifdef __cplusplus
}
#endif


#endif
