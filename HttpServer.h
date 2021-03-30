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

//#include "utils.h"

#include "Request.h"
#include "Response.h"

class HttpServer {

	public:

		explicit HttpServer(int port);
		HttpServer(int port, std::string  bindAddress);
		~HttpServer();

		void describe() const;

		[[noreturn]] void start();

	protected:

		SOCKET masterSocket;

	private:

		void acceptRequest();

		void handleRoute(SOCKET client);

		static void respond(Request *request, Response *response, const Compteur& compteur);

		static std::string jsonify(const Compteur& cpt);

		void http_get_counter(Request *request, Response *response, const std::string& name = "");
		void http_get_all_counters(Request *request, Response *response);
		void http_head(Request *request, Response *response);
		void http_post_counter(Request *request, Response *response);
		void http_put_counter(Request *request, Response *response);
		void http_del_counter(Request *request, Response *response);

		int m_port;
		std::string m_bindAddr;

		std::vector<Compteur> m_compteurs;

		Compteur getCounter(const std::string &name);

		bool counterExists(const std::string &name);

};


#endif //HTTP_COUNTER_HTTPSERVER_H
