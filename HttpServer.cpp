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
	Compteur cptEtoile("etoile", 10, true);

	m_compteurs.push_back(cptCarotte);
	m_compteurs.push_back(cptEtoile);
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
	}

	response.write();

	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	closesocket(client);
}

void HttpServer::http_get_counter(Request *request, Response *response) {
	//Get a specific counter

	rj::Document doc;
	doc.SetObject();

	rj::Document::AllocatorType &allocator = doc.GetAllocator();

	rj::Value arr(rj::kArrayType);

	for(auto& cpt : m_compteurs) {
		if(cpt.getNom() == request->getCounterName()) {
			//If the counter exist
			cpt.inc(); //Increment counter value
		}
	}
}

void HttpServer::http_get_all_counters(Request *request, Response *response) {
	//
}

void HttpServer::http_post_counter(Request *request, Response *response) {

}

void HttpServer::http_put_counter(Request *request, Response *response) {

}

void HttpServer::http_del_counter(Request *request, Response *response) {

}
