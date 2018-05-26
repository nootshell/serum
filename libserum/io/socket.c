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


#include "./socket.h"

#if (!LS_WINDOWS)
#	include <netdb.h>
#	include <unistd.h>
#	include <arpa/inet.h>
#endif

#include <string.h>




#define LS_SOCKET_FMASK_SOCKTYPE			(LS_SOCKET_TCP | LS_SOCKET_UDP)




/* STATIC */

#if (LS_WINDOWS)
static WSADATA __wsa_data;
#endif

static size_t __n_init_socks = 0;




void
LS_ATTR_NONNULL
static __socket_clear_struct(ls_socket_t *const pstruct) {
	memset(pstruct, 0, sizeof(*pstruct));
	pstruct->descriptor = LS_INVALID_SOCKFD;
	pstruct->flags |= LS_SOCKET_INITIALIZED;
}



ls_bool_t
static __socket_incr() {
	if (__n_init_socks == SIZE_MAX) {
		ls_debug("Number of initialized sockets reached its maximum.");
		return false;
	}

#if (LS_WINDOWS)
	if (__n_init_socks == 0) {
		const uint32_t res = WSAStartup(0x0202, &__wsa_data);
		if (res != 0) {
			ls_debugf("WSAStartup() startup failed: %u", res);
			return false;
		}
	}
#endif

	++__n_init_socks;
	return true;
}


ls_bool_t
static __socket_decr() {
	if (__n_init_socks == 0) {
		ls_debug("Number of initialized sockets is already zero.");
		return false;
	}

#if (LS_WINDOWS)
	if (__n_init_socks == 1) {
		const uint32_t res = WSACleanup();
		if (res != 0) {
			ls_debugf("WSACleanup() failed: %u", res);
			return false;
		}
	}
#endif

	--__n_init_socks;
	return true;
}



ls_bool_t
LS_ATTR_NONNULL
static __socket_validate_fd(ls_sockfd_t *const fd) {
#if (LS_WINDOWS)
#	error Need impl.
#else
	if (*fd != -1) {
		return true;
	}
#endif

	*fd = LS_INVALID_SOCKFD;
	return false;
}



int
static __socket_close(const ls_sockfd_t fd) {
#if (LS_WINDOWS)
#	error Need impl.
#else
		return close(fd);
#endif
}



ls_result_t
static __socket_init(ls_sockfd_t *const restrict out_descriptor, struct addrinfo **const restrict ai_root, struct addrinfo **const restrict ai_selected, const char *const restrict node, const char *const restrict service, const uint16_t port, const uint32_t flags) {
	if (out_descriptor == NULL || ai_root == NULL) {
		return_e(LS_E_NULL);
	}

	if (service == NULL && port == 0) {
		return_e(LS_E_INVALID);
	}


	if (__socket_incr() != true) {
		return_e(LS_E_INITIALIZATION);
	}


	const ls_bool_t
		hard_set_port = (service == NULL),
		want_serve = LS_FLAG(flags, LS_SOCKET_SERVER);


	// TODO: base on configuration (flags)
	int ai_flags = (want_serve ? AI_PASSIVE : 0),
		ai_family = AF_UNSPEC,
		ai_socktype = SOCK_STREAM,
		ai_protocol = IPPROTO_TCP;

	if (LS_FLAG(flags, LS_SOCKET_TCP)) {
		// Default
	} else if (LS_FLAG(flags, LS_SOCKET_UDP)) {
		ai_socktype = SOCK_DGRAM;
		ai_protocol = IPPROTO_UDP;
	} else {
		return_e(LS_E_INVALID);
	}


	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));

	hints.ai_flags = ai_flags;
	hints.ai_family = ai_family;
	hints.ai_socktype = ai_socktype;
	hints.ai_protocol = ai_protocol;


	struct addrinfo *root = NULL;
	int result = getaddrinfo(node, service, &hints, &root);
	if (result) {
		ls_debugfe("getaddrinfo() returned %i %i", result, errno);
		__socket_decr();
		return_e(LS_E_FAILURE);
	}


#if (LS_DEBUG)
	char dbg_addr[INET6_ADDRSTRLEN];
#endif

	ls_sockfd_t sockfd = LS_INVALID_SOCKFD;
	const uint16_t nbo_port = htons(port);


	struct addrinfo *ai_ptr;
	for (ai_ptr = root; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {
		if (hard_set_port) {
			((struct sockaddr_in*)ai_ptr->ai_addr)->sin_port = nbo_port;
		}

#if (LS_DEBUG)
		inet_ntop(ai_ptr->ai_family, ai_ptr->ai_addr, dbg_addr, ai_ptr->ai_addrlen);
#endif

		sockfd = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol);
		if (!__socket_validate_fd(&sockfd)) {
			result = errno;
			ls_debugfe("Failed to create socket: address=[%s] result=%i", dbg_addr, result);
			continue;
		}

		result = 0;
		if (want_serve) {
			result = bind(sockfd, ai_ptr->ai_addr, ai_ptr->ai_addrlen);
			if (result == 0) {
				result = listen(sockfd, SOMAXCONN); // TODO: SOMAXCONN configurable
				if (result == 0) {
					// Success
					ls_debugf("Started listening as server: address=[%s]", dbg_addr);
					break;
				} else {
					ls_debugfe("Failed to listen: %i", result);
				}
			} else {
				ls_debugfe("Failed to bind: %i", result);
			}
		} else {
			result = connect(sockfd, ai_ptr->ai_addr, ai_ptr->ai_addrlen);
			if (result == 0) {
				// Success
				ls_debugf("Connected as client: address=[%s]", dbg_addr);
				break;
			} else {
				ls_debugfe("Failed to connect: %i", result);
			}
		}

		result = __socket_close(sockfd);
		if (result != 0) {
			ls_debugfe("Failed to close socket: %i", result);
		}

		ls_debugfe("Failed to set up: address=[%s]", dbg_addr);
	}


	if (ai_ptr != NULL) {
		/* Non-null ai_ptr indicates we broke out of the loop, meaning our calls were successful. */

		*out_descriptor = sockfd;
		*ai_root = root;

		if (ai_selected) {
			*ai_selected = ai_ptr;
		}

		return LS_E_SUCCESS;
	}


	return_e(LS_E_FAILURE);
}


ls_result_t
static __socket_clear(ls_sockfd_t *const restrict descriptor, struct addrinfo **const restrict ai_root, struct addrinfo **const restrict ai_selected, const uint32_t flags) {
	if (descriptor == NULL || ai_root == NULL) {
		return_e(LS_E_NULL);
	}


	if (*descriptor != LS_INVALID_SOCKFD) {
		const int result = __socket_close(*descriptor);

		if (result == 0) {
			*descriptor = LS_INVALID_SOCKFD;
		} else {
			ls_debugfe("Failed to close socket: %i", result);
		}
	}

	if (*ai_root != NULL) {
		freeaddrinfo(*ai_root);
		*ai_root = NULL;
	}

	if (ai_selected != NULL && *ai_selected != NULL) {
		*ai_selected = NULL;
	}


	return LS_E_SUCCESS;
}


ls_bool_t
LS_ATTR_NONNULL
static __socket_check_initialized(const ls_socket_t *const socket) {
	if (socket->ai_root == NULL || socket->ai_selected == NULL) {
		return false;
	}

	if (socket->descriptor == LS_INVALID_SOCKFD) {
		return false;
	}

	if (!LS_FLAG(socket->flags, LS_SOCKET_INITIALIZED)) {
		return false;
	}

	return true;
}


ls_bool_t
LS_ATTR_NONNULL
static __socket_check_active(const ls_socket_t *const socket, const ls_bool_t check_initialized) {
	if (check_initialized && !__socket_check_initialized(socket)) {
		return false;
	}

	if (!LS_FLAG(socket->flags, LS_SOCKET_READY)) {
		return false;
	}

	return true;
}

/* END STATIC */




/* NON-STATIC */

ls_result_t
ls_socket_init_fd(ls_socket_t *const socket, const ls_sockfd_t descriptor) {
	if (socket == NULL) {
		return_e(LS_E_NULL);
	}

	__socket_clear_struct(socket);
	socket->descriptor = descriptor;

	return LS_E_SUCCESS;
}


ls_result_t
ls_socket_init(ls_socket_t *const socket) {
	if (socket == NULL) {
		return_e(LS_E_NULL);
	}

	__socket_clear_struct(socket);

	return LS_E_SUCCESS;
}


ls_result_t
ls_socket_clear(ls_socket_t *const socket) {
	const ls_result_t result = ls_socket_stop(socket);

	if (result == LS_E_SUCCESS || result == LS_E_ALREADY) {
		__socket_clear_struct(socket);
		return LS_E_SUCCESS;
	} else {
		return result;
	}
}




ls_result_t
ls_socket_accept_fd_ex(ls_socket_t *const restrict socket, ls_sockfd_t *const restrict out_descriptor, struct sockaddr *const restrict out_sockaddr, socklen_t *const restrict inout_sockaddrlen) {
	if (socket == NULL || out_descriptor == NULL) {
		return_e(LS_E_NULL);
	}

	if (socket->descriptor == LS_INVALID_SOCKFD) {
		return_e(LS_E_UNINITIALIZED);
	}

	if (!LS_FLAG(socket->flags, LS_SOCKET_SERVER)) {
		return_e(LS_E_INVALID);
	}

	if (!LS_FLAG(socket->flags, LS_SOCKET_READY)) {
		return_e(LS_E_STATE);
	}


	ls_sockfd_t sockfd = accept(socket->descriptor, out_sockaddr, inout_sockaddrlen);
	if (!__socket_validate_fd(&sockfd)) {
		const int result = errno;
		ls_debugfe("Failed to accept connection: %i", result);
		return_e(LS_E_FAILURE);
	}


	*out_descriptor = sockfd;
	return LS_E_SUCCESS;
}

ls_result_t
ls_socket_accept_fd(ls_socket_t *const restrict socket, ls_sockfd_t *const restrict out_descriptor) {
	return ls_socket_accept_fd_ex(
		socket,
		out_descriptor,
		NULL,
		NULL
	);
}


ls_result_t
ls_socket_accept_ex(ls_socket_t *const restrict socket, ls_socket_t *const restrict out_client, struct sockaddr *const restrict out_sockaddr, socklen_t *const restrict inout_sockaddrlen) {
	ls_sockfd_t sockfd;

	ls_result_t result = ls_socket_accept_fd_ex(socket, &sockfd, out_sockaddr, inout_sockaddrlen);
	if (result != LS_E_SUCCESS) {
		return result;
	}

	result = ls_socket_init_fd(out_client, sockfd);
	if (result != LS_E_SUCCESS) {
		ls_socket_clear(out_client);
		return result;
	}

	return LS_E_SUCCESS;
}

ls_result_t
ls_socket_accept(ls_socket_t *const restrict socket, ls_socket_t *const restrict out_client) {
	return ls_socket_accept_ex(
		socket,
		out_client,
		NULL,
		NULL
	);
}




ls_result_t
ls_socket_start_tcp(ls_socket_t *const restrict socket, const char *const restrict node, const char *const restrict service, const uint16_t port) {
	if (socket == NULL) {
		return_e(LS_E_NULL);
	}


	const ls_result_t result = __socket_init(
		&socket->descriptor,
		&socket->ai_root,
		&socket->ai_selected,
		node, service, port,
		((socket->flags & ~LS_SOCKET_FMASK_SOCKTYPE) | LS_SOCKET_TCP)
	);

	if (result == LS_E_SUCCESS) {
		socket->flags |= LS_SOCKET_READY;
	}

	return result;
}


ls_result_t
ls_socket_stop(ls_socket_t *const socket) {
	if (socket == NULL) {
		return_e(LS_E_NULL);
	}


	return __socket_clear(
		&socket->descriptor,
		&socket->ai_root,
		&socket->ai_selected,
		socket->flags
	);
}




ls_result_t
ls_socket_write(ls_socket_t *const restrict socket, const void *const restrict data, const size_t length) {
	if (socket == NULL || data == NULL) {
		return_e(LS_E_NULL);
	}

	if (length == 0) {
		// TODO: ping?
		return_e(LS_E_NOOP);
	}

	if (length > SSIZE_MAX) {
		return_e(LS_E_SIZE);
	}

	if (!__socket_check_active(socket, true)) {
		return_e(LS_E_STATE);
	}


	const size_t mtu = socket->mtu;
	ssize_t result;
	size_t sent = 0, msglen = 0;
	do {
		msglen = (length - sent);
		if (mtu != 0 && msglen > mtu) {
			msglen = mtu;
		}

		ls_debugf("Socket write: mtu=[%" PRIuPTR "] buff=[%" PRIXPTR "] len=[%" PRIuPTR "] offset=[%" PRIuPTR "] msglen=[%" PRIuPTR "]", mtu, data, length, sent, msglen);

		result = send(socket->descriptor, (((char*)data) + sent), msglen, 0);
		if (result < 0) {
			// Error

			ls_debugfe("Socket write failed: %" PRIiPTR " %i", result, errno);

			// TODO: retry?
			return_e(LS_E_FAILURE);
		}
#if (LS_DEBUG)
		else if (result == 0) {
			ls_debug("send() returned 0");
		}
#endif

		sent += result;
	} while (sent < length);


	return LS_E_SUCCESS;
}


ls_result_t
ls_socket_read(ls_socket_t *const restrict socket, void *const restrict buffer, const size_t max_length, size_t *const out_size) {
	if (socket == NULL || buffer == NULL) {
		return_e(LS_E_NULL);
	}

	if (max_length == 0) {
		return_e(LS_E_SIZE);
	}

	if (!__socket_check_active(socket, true)) {
		return_e(LS_E_STATE);
	}


	ssize_t received = recv(socket->descriptor, buffer, max_length, 0);
	if (received < 0) {
		// Error

		if (out_size != NULL) {
			*out_size = 0;
		}

		ls_debugfe("Socket read failed; %" PRIiPTR " %i", received, errno);
		return_e(LS_E_FAILURE);
	}

	if (out_size != NULL) {
		*out_size = (size_t)received;
	}


	return LS_E_SUCCESS;
}

/* END NON-STATIC */
