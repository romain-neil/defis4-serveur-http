//
// Created by Romain Neil on 26/03/2021.
//

#pragma once

#ifndef HTTP_COUNTER_HTTPSERVER_H
#define HTTP_COUNTER_HTTPSERVER_H

#include <cmath>
#include <functional>
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <streambuf>
#include <filesystem>
#include <optional>

#include "Request.h"
#include "Response.h"

namespace fs = std::filesystem;

class HttpServer {

	public:

		explicit HttpServer(int port, std::string bindAddress = "127.0.0.1", bool ipv6 = false, std::string bindHost = "localhost");

		~HttpServer();

		void describe() const;

		void listen(std::string url);

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

		void respond(Request *request, Response *response, const Compteur &compteur);

		void respond(Request *request, Response *response, const std::vector<Compteur> &counters);

		static std::string jsonify(const Compteur &cpt);

		void http_get_counter(Request *request, Response *response, const std::string &name = "");

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
		 * @param name the counter name to get
		 * @return the counter
		 */
		std::optional<std::reference_wrapper<Compteur>> getCounter(const std::string &name);

		/**
		 * Return the special counter "etoile"
		 * @return The counter "etoile"
		 */
		Compteur getStarCounter();

		bool counterExists(const std::string &name);

};


#endif //HTTP_COUNTER_HTTPSERVER_H
