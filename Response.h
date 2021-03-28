//
// Created by Romain Neil on 28/03/2021.
//

#ifndef HTTP_COUNTER_RESPONSE_H
#define HTTP_COUNTER_RESPONSE_H

#include <string>
#include <vector>
#include <sstream>
#include <vector>

#include "utils.h"

/**
 * Représente la réponse pour un client
 */
class Response {

	public:

		explicit Response(SOCKET socket);
		~Response() = default;

		void write() const;

		void setHeader(const std::string& headerName, const std::string& headerValue);

		void setHttpStatusCode(int code);

	private:

		unsigned int m_httpCode = 200;

		SOCKET m_socket;

		std::vector<std::string> m_responseHeader{};
		std::stringstream m_response;

};


#endif //HTTP_COUNTER_RESPONSE_H
