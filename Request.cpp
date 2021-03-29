//
// Created by Romain Neil on 28/03/2021.
//

#include <iostream>
#include "Request.h"

Request::Request(SOCKET client, char buffer[]) : m_socket(client) {
	m_url = Http::Url::UNKNOWN;

	//Parse http request
	std::string req = std::string(buffer);

	//Detect http verb
	if(req.find("GET") == 0) {
		m_method = Http::GET;
	} else if (req.find("POST") == 0) {
		m_method = Http::POST;
	} else if(req.find("PUT") == 0) {
		m_method = Http::PUT;
	} else if(req.find("DELETE") == 0) {
		m_method = Http::DELETE_VERB;
	} else {
		m_method = Http::BAD_METHOD;
		m_isValidRequest = false;
	}

	//Detect http url (if any)
	int url_start_pos = req.find('/');
	std::string req_cpy = std::string(req);
	std::string url;

	if(url_start_pos != std::string::npos) {
		//delete useless chars from line
		req_cpy.erase(0, url_start_pos);

		//Start reading url
		for(auto c : req_cpy) {
			//if the char is a space, admitting we reach the end of the url
			if(c == ' ') {
				break;
			}

			url.push_back(c);
		}
	}

	m_clientRequest = url;

	std::cout << "Url : " << url << std::endl;

	//Detect request headers
	req.erase(0, req.find("\r\n")); //Erase first line

	std::istringstream iss(req);

	for(std::string line; std::getline(iss, line);) {
		//For each line of headers
		if(!line.empty()) { //If we don't reach the end of the headers
			int sep = line.find(':');

			if(sep != std::string::npos) {
				//Header found !
				addHeader(line.substr(0, sep), (line.substr(sep + 2, line.size())));
			}
		} else {
			break; //break, dont parse input for the moment
		}
	}
}

void Request::process() {
	if(m_isValidRequest) {
		determineRoute();
	}
}

std::string Request::getCounterName() {
	std::string tmp = m_clientRequest;

	//We delete the first '/' on the url
	tmp.erase(0, 1);

	return tmp;
}

void Request::determineRoute() {
	if(m_clientRequest == "/" && m_clientRequest.length() == 1) {
		//show all counters
		m_methodEdit = true;
	}

	switch(m_method) {
		case Http::GET:
			if(m_methodEdit) { //If we show all counters
				m_url = Http::Url::GET_ALL_CPT;
			} else {
				m_url = Http::Url::GET_SPECIFIC_CPT;
			}
			break;
		case Http::POST:
			m_url = Http::Url::POST_CREATE_CPT;
			break;
		case Http::PUT:
			m_url = Http::Url::PUT_EDIT_CPT;
			break;
		case Http::DELETE_VERB:
			m_url = Http::Url::DELETE_CPT;
			break;
		default:
			//Normaly not reached
			m_url = Http::UNKNOWN;
			break;
	}
}

std::string Request::getHeader(const std::string& name) {
	for(const auto& header : m_finalHeaders) {
		if(header.first == name) {
			return header.second;
		}
	}

	return std::string();
}

void Request::addHeader(const std::string& name, const std::string& val) {
	m_finalHeaders.insert({name, val});
}
