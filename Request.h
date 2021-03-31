//
// Created by Romain Neil on 28/03/2021.
//

#ifndef HTTP_COUNTER_REQUEST_H
#define HTTP_COUNTER_REQUEST_H

#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>

#include "rapidjson/document.h"

#include "utils.h"

namespace rj = rapidjson;

class Request {

	public:

		Request(SOCKET client, char buffer[]);

		/**
		 * Process client request
		 */
		void process();

		Http::Url getUrl() {
			return m_url;
		}

		/**
		 * Get all headers
		 * @return all headers parsed
		 */
		std::unordered_map<std::string, std::string> getAllHeaders() {
			return m_headers;
		}

		std::string getParam(const std::string &name);

		std::string getHeader(const std::string& name);

		Http::Verb getVerb() {
			return m_method;
		}

		std::string getCounterName();

	private:

		/**
		 * Extract headers from request
		 */
		void extractHeaders();

		/**
		 * Determine the route from the client request
		 */
		void determineRoute();

		/**
		 * Add the header with his name and value
		 * @param name name of the header
		 * @param val value of the header
		 */
		void addHeader(const std::string& name, const std::string& val);

		/**
		 * Append param
		 * @param name name of the parameter
		 * @param val value of the parameter
		 */
		void addParam(const std::string &name, const std::string &val);

		/**
		 * Parse json params
		 */
		void parseJsonParams();

		/**
		 * Parse form params
		 */
		void parseFormParams();

		/**
		 * Sanitarize json string
		 */
		void sanitarizeJson();

		Http::Verb m_method;
		Http::Url m_url;

		bool m_isValidRequest = true;
		bool m_methodEdit = false;

		SOCKET m_socket;

		std::string m_clientRequest;
		std::string m_req;

		/**
		 * Request headers
		 */
		std::unordered_map<std::string, std::string> m_headers;

		/**
		 * Request params
		 */
		std::unordered_map<std::string, std::string> m_params;

};


#endif //HTTP_COUNTER_REQUEST_H
