//
// Created by Romain Neil on 26/03/2021.
//

#ifndef HTTP_COUNTER_HTTPSERVER_H
#define HTTP_COUNTER_HTTPSERVER_H

#include <functional>
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <filesystem>

#include "Request.h"
#include "Response.h"

namespace fs = std::filesystem;

class HttpServer {

	public:

		HttpServer(int port, std::string  bindAddress = "127.0.0.0", std::string bindHost = "localhost");
		~HttpServer();

		void describe() const;

		[[noreturn]] void start();

	protected:

		SOCKET masterSocket;

	private:

		/**
		 * Load html pages into memory
		 */
		void loadHtmlContent();

		void acceptRequest();

		void handleRoute(SOCKET client);

		static void respond(Request *request, Response *response, const Compteur& compteur);
		void respond(Request *request, Response *response, const std::vector<Compteur> &counters);

		static std::string jsonify(const Compteur& cpt);

		void http_get_counter(Request *request, Response *response, const std::string& name = "");
		void http_get_all_counters(Request *request, Response *response);
		void http_post_counter(Request *request, Response *response);
		void http_put_counter(Request *request, Response *response);
		void http_del_counter(Request *request, Response *response);

		int m_port;
		std::string m_bindAddr;
		std::string m_bindHost;

		std::string m_htmlHeader; //Html header
		std::string m_htmlFooter; //Html footer

		std::vector<Compteur> m_compteurs;

		/**
		 * WARNING: Be sure to call counterExist before
		 * @param name the counter name to get
		 * @return the counter
		 */
		Compteur getCounter(const std::string &name);

		bool counterExists(const std::string &name);

};


#endif //HTTP_COUNTER_HTTPSERVER_H
