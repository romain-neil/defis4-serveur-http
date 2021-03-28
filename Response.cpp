//
// Created by Romain Neil on 28/03/2021.
//

#include "Response.h"

Response::Response(SOCKET socket) : m_socket(socket) {
	m_response << "HTTP/1.1 ";
}

void Response::write(const std::string& data) {
	setHeader("Server", "TinyWebServer v1.0");
	setHeader("Content-Length", std::to_string(data.length()));

	//Write content to socket
	send(m_socket, m_response.str().c_str(), m_response.str().length(), 0);
}

void Response::setContentType(const std::string &contentType) {
	setHeader("Content-Type", contentType);
}

void Response::setHeader(const std::string& headerName, const std::string& headerValue) {
	m_response << headerName << ": " << headerValue << std::endl;
}

void Response::setHttpStatusCode(int code) {
	m_response << code << " " << HttpStatus::reasonPhrase(code) << std::endl;
}
