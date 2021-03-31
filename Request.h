//
// Created by Romain Neil on 28/03/2021.
//

#ifndef HTTP_COUNTER_REQUEST_H
#define HTTP_COUNTER_REQUEST_H

#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>

#include "utils.h"

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
			return m_finalHeaders;
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

		void determineRoute();

		void addHeader(const std::string& name, const std::string& val);

		void addParam(const std::string &name, const std::string &val);

		/**
		 * Parse json params
		 */
		void parseJsonParams();

		/**
		 * Parse form params
		 */
		void parseFormParams();

		Http::Verb m_method;
		Http::Url m_url;

		bool m_isValidRequest = true;
		bool m_methodEdit = false;

		SOCKET m_socket;

		std::string m_clientRequest;
		std::string m_req;

		/**
		 * Temp string stream
		 */
		std::stringstream m_ss;

		/**
		 * Request params
		 */
		std::unordered_map<std::string, std::string> m_params;

		std::unordered_map<std::string, std::string> m_finalHeaders;

};


#endif //HTTP_COUNTER_REQUEST_H
