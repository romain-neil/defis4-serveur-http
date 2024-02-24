//
// Created by Romain Neil on 28/03/2021.
//

#pragma once

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

		/**
		 * Send error to client
		 */
		void write();

		void sendNotFound();

		/**
		 * Implement HEAD Request
		 */
		void dryRun();

		void write(const std::string &data);

		void setContentType(const std::string &contentType);

		void setHeader(const std::string &headerName, const std::string &headerValue);

		void setHttpStatusCode(int code);

		/**
		 * Close the request and send to the client a 204 status
		 */
		void close();

		/**
		 * Send html response
		 */
		void html();

		/**
		 * Send json response
		 */
		void json();

	private:

		SOCKET m_socket;

		bool m_isDryRun = false;

		std::stringstream m_response;

};


#endif //HTTP_COUNTER_RESPONSE_H
