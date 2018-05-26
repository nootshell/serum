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


#ifndef __LS_IO_SOCKET_H
#define __LS_IO_SOCKET_H




#include "../core/setup.h"

#if (LS_WINDOWS)
#	include <winsock2.h>
#	include <ws2tcpip.h>

typedef SOCKET ls_sockfd_t;
#else
#	include <sys/types.h>
#	include <sys/socket.h>

typedef int ls_sockfd_t;
#endif




#define LS_INVALID_SOCKFD					~((ls_sockfd_t)0)


#define LS_SOCKET_INITIALIZED				0x00000001
#define LS_SOCKET_READY						0x00000002

#define LS_SOCKET_SERVER					0x00000008

#define LS_SOCKET_TCP						0x00000100
#define LS_SOCKET_UDP						0x00000200




typedef struct ls_socket {
	struct addrinfo *ai_root;
	struct addrinfo *ai_selected;
	size_t mtu;
	ls_sockfd_t descriptor;
	uint32_t flags;
} ls_socket_t;




#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_socket_init_fd(ls_socket_t *const socket, const ls_sockfd_t descriptor);
	LSAPI ls_result_t ls_socket_init(ls_socket_t *const socket);
	LSAPI ls_result_t ls_socket_clear(ls_socket_t *const socket);

	LSAPI ls_result_t ls_socket_start_tcp(ls_socket_t *const restrict socket, const char *const restrict node, const char *const restrict service, const uint16_t port);
	LSAPI ls_result_t ls_socket_stop(ls_socket_t *const socket);

	LSAPI ls_result_t ls_socket_accept_fd_ex(ls_socket_t *const restrict socket, ls_sockfd_t *const restrict out_descriptor, struct sockaddr *const restrict out_sockaddr, socklen_t *const restrict inout_sockaddrlen);
	LSAPI ls_result_t ls_socket_accept_fd(ls_socket_t *const restrict socket, ls_sockfd_t *const restrict out_descriptor);
	LSAPI ls_result_t ls_socket_accept_ex(ls_socket_t *const restrict socket, ls_socket_t *const restrict out_client, struct sockaddr *const restrict out_sockaddr, socklen_t *const restrict inout_sockaddrlen);
	LSAPI ls_result_t ls_socket_accept(ls_socket_t *const restrict socket, ls_socket_t *const restrict out_client);

	LSAPI ls_result_t ls_socket_write(ls_socket_t *const restrict socket, const void *const restrict data, const size_t length);
	LSAPI ls_result_t ls_socket_read(ls_socket_t *const restrict socket, void *const restrict buffer, const size_t max_length, size_t *const out_size);

#ifdef __cplusplus
}
#endif




#endif
