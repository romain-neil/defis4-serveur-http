//
// Created by super on 28/03/2021.
//

#ifndef HTTP_COUNTER_UTILS_H
#define HTTP_COUNTER_UTILS_H

#include <string>
#include "http_status_codes.h"
#include "Compteur.h"

namespace Http {

	enum Verb {
		GET = 0,
		POST,
		PUT,
		DELETE_VERB,

		BAD_METHOD //Keep last
	};

	/**
	 * Liste des différentes routes
	 */
	enum Url {
		UNKNOWN = 1,

		GET_ALL_CPT,
		GET_SPECIFIC_CPT,

		POST_CREATE_CPT,

		PUT_EDIT_CPT,

		DELETE_CPT
	};

}

#if defined(_WIN32)

#include <winsock2.h>

#pragma comment(lib, "ws2_32")

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#endif

#endif //HTTP_COUNTER_UTILS_H
