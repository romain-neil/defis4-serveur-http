//
// Created by Romain Neil on 26/03/2021.
//

#include "HttpServer.h"

HttpServer::HttpServer(int port) : HttpServer(port, "127.0.0.1") {}

HttpServer::HttpServer(int port, std::string  bindAddress) : m_port(port), m_bindAddr(std::move(bindAddress)) {
#if defined(_WIN32)
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2,2), &WSAData);
#endif

	masterSocket = socket(AF_INET, SOCK_STREAM, 0);

	if(masterSocket == INVALID_SOCKET) {
		perror("failed to bind");

#if defined(_WIN32)
		const DWORD lastError = WSAGetLastError();

		puts("error: " + lastError);
#endif
	}

	sockaddr_in sin = {0};
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	if(bind(masterSocket, reinterpret_cast<sockaddr*>(&sin), sizeof sin) == SOCKET_ERROR) {
		perror("unabel to bind !");
		exit(errno);
	}

	puts("bind done");

	if(listen(masterSocket, 32) == SOCKET_ERROR) {
		perror("listen()");
		exit(errno);
	}

	Compteur cptCarotte("carotte", 10);

	m_compteurs.push_back(cptCarotte);
}

HttpServer::~HttpServer() {
	m_compteurs.clear();
}

void HttpServer::describe() const {
	std::cout << "The server is listening on port " << m_port << std::endl;
}

[[noreturn]] void HttpServer::start() {
	while(true) {
		acceptRequest();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void HttpServer::acceptRequest() {
	sockaddr_in csin = {0};
	int sinsize = sizeof csin;
	SOCKET csock_tmp = accept(masterSocket, reinterpret_cast<sockaddr*>(&csin), &sinsize);

	if(csock_tmp != INVALID_SOCKET) {
		//handle client request here
		handleRoute(csock_tmp);
	} else {
		perror("Invalid socket !");
	}
}

void HttpServer::handleRoute(SOCKET client) {
	char buffer[1024];

	recv(client, buffer, 1024, 0);

	Request request(client, buffer);
	request.process();

	Http::Url requestedUrl = request.getUrl();
	Http::Verb method = request.getVerb();

	Response response(client);

	if(method == Http::HEAD) {
		response.dryRun();
		method = Http::GET;
	}

	if(method == Http::GET) {
		if(requestedUrl == Http::Url::GET_ALL_CPT) {
			//show all counters, formatted
			http_get_all_counters(&request, &response);
		} else {
			http_get_counter(&request, &response);
		}
	} else if(method == Http::POST) {
		http_post_counter(&request, &response);
	} else if(method == Http::PUT) {
		http_put_counter(&request, &response);
	} else if(method == Http::DELETE_VERB) {
		http_del_counter(&request, &response);
	} else {
		//Bad request
		response.setHttpStatusCode(400);
		response.write();
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	closesocket(client);
}

void HttpServer::respond(Request *request, Response *response, const Compteur& cpt) {
	std::string acceptHeader = request->getHeader("Accept");
	std::string resp;

	response->setHttpStatusCode(200);

	//Determine server response format
	if(Http::RequestUtil::AcceptEverything(acceptHeader)) {
		//We respond html
		response->setContentType("text/html; charset=utf-8");
		resp = "<p>" + cpt.getNom() + " : " + std::to_string(cpt.getVal()) + "</p>";
	} else if(Http::RequestUtil::AcceptJson(acceptHeader)) {
		//Some json
		response->setContentType("application/json");
		resp = jsonify(cpt);
	} else {
		//Respond text
		response->setContentType("text/plain");
		resp = cpt.getNom() + ": " + std::to_string(cpt.getVal());
	}

	response->write(resp);
}

std::string HttpServer::jsonify(const Compteur& cpt) {
	return "{'compteur': '" + cpt.getNom() + "', 'value': '" + std::to_string(cpt.getVal()) + "'}";
}

void HttpServer::http_get_counter(Request *request, Response *response, const std::string& name) {
	//Get a specific counter
	std::string counter;

	if(name.empty()) {
		counter = request->getCounterName();
	} else {
		counter = name;
	}

	//If this is the special counter
	//Count all val of counters
	if(counter == "etoile") {
		Compteur etoile("etoile", 0);

		for(auto &cpt : m_compteurs) {
			etoile.inc(cpt.getVal());
		}

		respond(request, response, etoile);
		return;
	} else {
		if(counterExists(counter)) {
			respond(request, response, getCounter(counter));
			return;
		}
	}

	//Counter not found
	response->sendNotFound();
}

void HttpServer::http_get_all_counters(Request *request, Response *response) {
	std::stringstream ss;
	ss << "{'counters': [";

	for(const auto& cpt : m_compteurs) {
		ss << "{'name': '" << cpt.getNom() << "', 'value': '" << cpt.getVal() << "'},";
	}

	ss << "]}";

	response->setHttpStatusCode(200);
	response->setContentType("application/json");
	response->write(ss.str());
}

void HttpServer::http_post_counter(Request *request, Response *response) {
	//Create a new counter
	if(request->getHeader("Content-Type").find("application/x-www-form-urlencoded") != std::string::npos) {
		//Header exists and are of expected type
		std::string newCounterName = request->getParam("name");

		if(!newCounterName.empty()) {
			if(!counterExists(newCounterName)) { //If the counter doesn't exists, we create it
				Compteur cpt(newCounterName, 1);
				m_compteurs.push_back(cpt);
			}

			//Redirect user to other page
			http_get_counter(request, response, newCounterName);
		} else {
			//Send an error to the client
			response->setHttpStatusCode(400);
			response->write();
		}
	}
}

void HttpServer::http_put_counter(Request *request, Response *response) {
	std::string counterName = request->getCounterName();

	if(!counterName.empty()) {
		for(auto &cpt : m_compteurs) {
			if(cpt.getNom() == counterName) {
				cpt.inc();

				response->setHttpStatusCode(204);
				response->write("");
				return;
			}
		}
	}

	//If any error
	response->setHttpStatusCode(400);
	response->write();
}

void HttpServer::http_del_counter(Request *request, Response *response) {
	std::string counterName = request->getCounterName();

	if(!counterName.empty()) {
		for(auto it = m_compteurs.begin(); it != m_compteurs.end(); it++) {
			if(it->getNom() == counterName) {
				m_compteurs.erase(it);

				response->setHttpStatusCode(200);
				response->write("");

				return;
			}
		}
	}

	//If any error
	response->setHttpStatusCode(400);
	response->write();
}

Compteur HttpServer::getCounter(const std::string &name) {
	for(auto& cpt : m_compteurs) {
		if(cpt.getNom() == name) {
			return cpt;
		}
	}
}

bool HttpServer::counterExists(const std::string &name) {
	for(const auto& cpt : m_compteurs) {
		if(cpt.getNom() == name) {
			return true;
		}
	}

	return false;
}
