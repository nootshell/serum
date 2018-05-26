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
	const uint32_t err = getaddrinfo(node, service, &hints, &root);
	if (err) {
		ls_debugfe("getaddrinfo() returned %u", err);
		__socket_decr();
		return_e(LS_E_FAILURE);
	}


#if (LS_DEBUG)
	char dbg_addr[INET6_ADDRSTRLEN];
#endif

	ls_sockfd_t sockfd = LS_INVALID_SOCKFD;
	const uint16_t nbo_port = htons(port);


	int result;
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

/* END STATIC */




/* NON-STATIC */

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
		return_e(result);
	}
}




ls_result_t
ls_socket_start_tcp(ls_socket_t *const restrict socket, const char *const restrict node, const char *const restrict service, const uint16_t port) {
	if (socket == NULL) {
		return_e(LS_E_NULL);
	}


	return __socket_init(
		&socket->descriptor,
		&socket->ai_root,
		&socket->ai_selected,
		node, service, port,
		((socket->flags & ~LS_SOCKET_FMASK_SOCKTYPE) | LS_SOCKET_TCP)
	);
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

/* END NON-STATIC */