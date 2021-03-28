//
// Created by Romain Neil on 28/03/2021.
//

#include "Response.h"

Response::Response(SOCKET socket) : m_socket(socket) {
	setHeader("Server", "TinyWebServer v1.0");
}

void Response::write() const {
	//Write content to socket
	send(m_socket, m_response.str().c_str(), m_response.str().length(), 0);
}

void Response::setHeader(const std::string& headerName, const std::string& headerValue) {
	m_response << headerName << ": " << headerValue << std::endl;
}

void Response::setHttpStatusCode(int code) {
	m_httpCode = code;
}
