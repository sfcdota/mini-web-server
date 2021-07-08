//
// Created by ljerrica on 29.06.2021.
//

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include "allowed_library_includes.hpp"


class Response {
public:
	Response();
	std::string SetResponseLine(const std::map<std::string, std::string> &request_line, conf &con);
	std::string GetStatusText(std::string code);
	std::string SendResponse();
	void	freeResponse();
// private:
	std::map<std::string, std::string> response_line;
	std::map<std::string,std::string> headers;
	std::map<std::string, std::string> status_codes;
	std::string body; //????????
	size_t status_code;
	conf s;
	bool failed;
};


#endif //WEBSERV_RESPONSE_HPP
