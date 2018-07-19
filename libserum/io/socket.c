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
#include <stdio.h>




#define LS_SOCKET_FMASK_SOCKTYPE			(LS_SOCKET_TCP | LS_SOCKET_UDP)




/* Windows is a special cookie. */
#if (LS_WINDOWS)
static WSADATA __wsa_data;
#endif

static size_t __n_init_socks = 0;




void
LS_ATTR_NONNULL
static __socket_clear_struct(ls_socket_t *const pstruct, const uint32_t flags) {
	memset(pstruct, 0, sizeof(*pstruct));
	pstruct->descriptor = LS_INVALID_SOCKFD;
	pstruct->flags = (flags | LS_SOCKET_INITIALIZED);
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
static __socket_init(ls_sockfd_t *const restrict out_descriptor, struct addrinfo **const restrict ai_root, struct addrinfo **const restrict ai_selected, const char *const restrict node, const char *restrict service, const uint16_t port, uint32_t *const restrict flags) {
	if (out_descriptor == NULL || ai_root == NULL) {
		return_e(LS_E_NULL);
	}

	if (service == NULL && port == 0) {
		return_e(LS_E_INVALID);
	}


	if (__socket_incr() != true) {
		return_e(LS_E_INITIALIZATION);
	}


	uint32_t __flags = ((flags != NULL) ? *flags : 0);


	const ls_bool_t want_serve = LS_FLAG(__flags, LS_SOCKET_SERVER);
	const uint32_t enabled = 1;


	// TODO: base on configuration (flags)
	int ai_flags = (want_serve ? AI_PASSIVE : 0),
		ai_family = AF_UNSPEC,
		ai_socktype = SOCK_STREAM,
		ai_protocol = IPPROTO_TCP;

	if (LS_FLAG(__flags, LS_SOCKET_TCP)) {
		// Default
	} else if (LS_FLAG(__flags, LS_SOCKET_UDP)) {
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


	char svcbuf[6];
	if (service == NULL) {
		if (snprintf(svcbuf, sizeof(svcbuf), "%u", port) < 0) {
			return_e(LS_E_CONVERSION);
		}

		service = svcbuf;
		hints.ai_flags |= AI_NUMERICSERV;
	}


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


	struct addrinfo *ai_ptr;
	for (ai_ptr = root; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {
#if (LS_DEBUG)
		inet_ntop(ai_ptr->ai_family, &((const struct sockaddr_in *const)ai_ptr->ai_addr)->sin_addr, dbg_addr, ai_ptr->ai_addrlen);
		ls_debugf("Trying address: host=[%s] %s=[%s]", dbg_addr, ((service == svcbuf) ? "port" : "serv"), service);
#endif

		sockfd = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol);
		if (!__socket_validate_fd(&sockfd)) {
			ls_debugfe("Failed to create socket: errno=[%i]", errno);
			continue;
		}

		result = 0;
		if (want_serve) {
			/* Set socket options before binding. */

#ifdef SO_REUSEADDR
			if (LS_FLAG(__flags, LS_SOCKOPT_REUSEADDR)) {
				if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled)) != 0) {
					ls_debugfe("Failed to set socket option: fd=[%i] opt=[%" PRIX32 "] errno=[%i]", sockfd, LS_SOCKOPT_REUSEADDR, errno);
					__flags &= ~LS_SOCKOPT_REUSEADDR;
				}
			}
#else
			LS_COMPILER_LOG("LS_SOCKOPT_REUSEADDR unavailable");
#endif

#ifdef SO_REUSEPORT
			if (LS_FLAG(__flags, LS_SOCKOPT_REUSEPORT)) {
				if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &enabled, sizeof(enabled)) != 0) {
					ls_debugfe("Failed to set socket option: fd=[%i] opt=[%" PRIX32 "] errno=[%i]", sockfd, LS_SOCKOPT_REUSEPORT, errno);
					__flags &= ~LS_SOCKOPT_REUSEPORT;
				}
			}
#else
			LS_COMPILER_LOG("LS_SOCKOPT_REUSEPORT unavailable");
#endif

			/* Bind, and listen. */
			result = bind(sockfd, ai_ptr->ai_addr, ai_ptr->ai_addrlen);
			if (result == 0) {
				if (ai_protocol != IPPROTO_UDP) {
					result = listen(sockfd, SOMAXCONN); // TODO: SOMAXCONN configurable
				}
				if (result == 0) {
					// Success
					ls_debug("Started listening as server");
					break;
				} else {
					ls_debugfe("Failed to listen: result=[%i] errno=[%i]", result, errno);
				}
			} else {
				ls_debugfe("Failed to bind: result=[%i] errno=[%i]", result, errno);
			}
		} else {
			result = connect(sockfd, ai_ptr->ai_addr, ai_ptr->ai_addrlen);
			if (result == 0) {
				// Success
				ls_debug("Connected as client");
				break;
			} else {
				ls_debugfe("Failed to connect: result=[%i] errno=[%i]", result, errno);
			}
		}

		result = __socket_close(sockfd);
		if (result != 0) {
			ls_debugfe("Failed to close socket: result=[%i] errno=[%i]", result, errno);
		}

		ls_debugfe("Failed to set up on address", 0);
	}


	if (ai_ptr != NULL) {
		/* Non-null ai_ptr indicates we broke out of the loop, meaning our calls were successful. */

		*out_descriptor = sockfd;
		*ai_root = root;

		if (ai_selected != NULL) {
			*ai_selected = ai_ptr;
		}

		if (flags != NULL) {
			*flags = __flags;
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
			ls_debugfe("Failed to close socket: result=[%i] errno=[%i]", result, errno);
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
		ls_debugfe("Socket not initialized: loc=[%" LS_PRIPTR "]", socket);
		return false;
	}

	if (!LS_FLAG(socket->flags, LS_SOCKET_READY)) {
		ls_debugfe("Socket not ready: loc=[%" LS_PRIPTR "]", socket);
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

	__socket_clear_struct(socket, 0);
	socket->descriptor = descriptor;

	return LS_E_SUCCESS;
}


ls_result_t
ls_socket_init(ls_socket_t *const socket, const uint32_t flags) {
	if (socket == NULL) {
		return_e(LS_E_NULL);
	}

	__socket_clear_struct(socket, flags);

	return LS_E_SUCCESS;
}


ls_result_t
ls_socket_clear(ls_socket_t *const socket) {
	const ls_result_t result = ls_socket_stop(socket);

	if (result == LS_E_SUCCESS || result == LS_E_ALREADY) {
		__socket_clear_struct(socket, 0);
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
		ls_debugfe("Failed to accept connection: errno=[%i]", errno);
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

	out_client->flags |= LS_SOCKET_READY;

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
ls_socket_start(ls_socket_t *const restrict socket, const char *const restrict node, const char *const restrict service, const uint16_t port) {
	if (socket == NULL) {
		return_e(LS_E_NULL);
	}


	const ls_result_t result = __socket_init(
		&socket->descriptor,
		&socket->ai_root,
		&socket->ai_selected,
		node, service, port,
		&socket->flags
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


	const ls_result_t result = __socket_clear(
		&socket->descriptor,
		&socket->ai_root,
		&socket->ai_selected,
		socket->flags
	);

	if (result == LS_E_SUCCESS) {
		socket->flags |= LS_SOCKET_CLOSED;
	}

	return result;
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

		ls_debugf("Socket write: fd=[%i] buff=[%" PRIXPTR "] len=[%" PRIuPTR "] offset=[%" PRIuPTR "] msglen=[%" PRIuPTR "] mtu=[%" PRIuPTR "]", socket->descriptor, data, length, sent, msglen, mtu);

		result = send(socket->descriptor, (((char*)data) + sent), msglen, 0);
		if (result < 0) {
			// Error

			ls_debugfe("Socket write failed: result=[%" PRIiPTR "] errno=[%i]", result, errno);

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

		ls_debugfe("Socket read failed: fd=[%i] recv=[%" PRIiPTR "] errno=[%i]", socket->descriptor, received, errno);
		return_e(LS_E_FAILURE);
	} else if (received == 0) {
		ls_debugfe("Socket closed: fd=[%i] errno=[%i]", socket->descriptor, errno);

		ls_socket_stop(socket);

		// Always specify LS_SOCKET_CLOSED on shutdown.
		socket->flags |= LS_SOCKET_CLOSED;

		return_e(LS_E_IO_CLOSE);
	}

	ls_debugf("Socket read: fd=[%i] buff=[%" PRIXPTR "] mlen=[%" PRIuPTR "] msglen=[%" PRIuPTR "]", socket->descriptor, buffer, max_length, received);

	if (out_size != NULL) {
		*out_size = (size_t)received;
	}


	return LS_E_SUCCESS;
}

/* END NON-STATIC */
