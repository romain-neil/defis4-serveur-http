//
// Created by Romain Neil on 28/03/2021.
//

#include <iostream>
#include "Request.h"

Request::Request(SOCKET client, char buffer[]) : m_socket(client) {
	m_url = Http::Url::UNKNOWN;

	//Parse http request
	m_req = std::string(buffer);

	//Detect http verb
	if(m_req.find("GET") == 0) {
		m_method = Http::GET;
	} else if(m_req.find("HEAD") == 0) {
		m_method = Http::HEAD;
	} else if (m_req.find("POST") == 0) {
		m_method = Http::POST;
	} else if(m_req.find("PUT") == 0) {
		m_method = Http::PUT;
	} else if(m_req.find("DELETE") == 0) {
		m_method = Http::DELETE_VERB;
	} else {
		m_method = Http::BAD_METHOD;
		m_isValidRequest = false;
	}

	//Detect http url (if any)
	int url_start_pos = m_req.find('/');
	std::string url;

	if(url_start_pos != std::string::npos) {
		//delete useless chars from line
		m_req.erase(0, url_start_pos);

		//Start reading url
		url = m_req.substr(0, m_req.find(' ')); //if the char is a space, admitting we reach the end of the url
	}

	m_clientRequest = url;

	std::cout << "Url : " << url << std::endl;

	m_req.erase(0, m_req.find("\r\n") + 2);

	extractHeaders();

	/* //TODO
	if(m_method == Http::POST) {
		//We need to check if all content were uploaded

		//if(std::stoi(getHeader("Content-Length")))
	}*/

	//Clear some unused stuff
	m_req.clear();
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

void Request::extractHeaders() {
	std::istringstream iss(m_req);
	std::istringstream str;

	//Extracting request headers
	for(std::string line; std::getline(iss, line);) { //Until the blank line
		//For each line of headers
		//Until we reach blank line
		if(!line.empty() && line.c_str()[0] != '{') { //If we don't reach the end of the headers
			int sep = line.find(':');

			if(sep != std::string::npos) {
				//Header found !
				line.pop_back();
				addHeader(line.substr(0, sep), (line.substr(sep + 2, line.size() - 1)));

				//Erase the line we just handle
				m_req.erase(0, line.size() + 2);
			}
		} else {
			break;
		}
	}

	std::string ctHd = getHeader("Content-Type");

	//Extracting request parameters
	if(!ctHd.empty()) {
		if(ctHd == "application/json") {
			parseJsonParams();
		} else if(ctHd == "application/x-www-form-urlencoded") {
			parseFormParams();
		}
	}
}

void Request::determineRoute() {
	if(m_clientRequest == "/" && m_clientRequest.length() == 1) {
		m_methodEdit = true; //show all counters
	}

	switch(m_method) {
		case Http::GET:
		case Http::HEAD:
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

std::string Request::getParam(const std::string &name) {
	for(const auto &param : m_params) {
		if(param.first == name) {
			return param.second;
		}
	}

	return std::string();
}

std::string Request::getHeader(const std::string& name) {
	for(const auto& header : m_headers) {
		if(header.first == name) {
			return header.second;
		}
	}

	return std::string();
}

void Request::addHeader(const std::string& name, const std::string& val) {
	m_headers.insert({name, val});
}

void Request::addParam(const std::string &name, const std::string &val) {
	m_params.insert({name, val});
}

void Request::parseJsonParams() {
	m_req.erase(0, 2);

	//Sanitarize json string
	sanitarizeJson();

	rj::Document doc;
	doc.Parse(m_req.c_str());

	assert(doc.HasMember("name"));
	assert(doc["name"].IsString());

	addParam("name", doc["name"].GetString());
}

void Request::parseFormParams() {
	//We have some params !
	int bytes = std::stoi(getHeader("Content-Length"));

	//We erase the blank line in the request
	m_req.erase(0, 2);

	int pos = m_req.find('=');

	addParam(m_req.substr(0, pos), m_req.substr(pos + 1, (bytes - (pos + 1))));
}

void Request::sanitarizeJson() {
	std::string json;

	//For each char in the json
	for(auto c : m_req) {
		if(c != '\r' && c != '\n' && c != '\t') {
			//We add that char to the final string
			json.push_back(c);
		}
	}

	m_req = json;
}
