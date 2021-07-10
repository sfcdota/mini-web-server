//
// Created by Knot Fonda on 7/10/21.
//

#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP

#include "allowed_library_includes.hpp"
#include "Server.hpp"

// Сервер отправляет данные программе CGI с помощью переменных среды

// Задача:
// Сформировать execve->env
// При использовании метода GET данные отправляются в переменную QUERY_STRING
// При использовании метода GET данные отправляются в body (CONTENT_LENGTH)
//

class CGI {
public:
	CGI();
	~CGI();
	void getEnv() const;
	void setEnv();
	void initialization(const std::map<std::string, std::string> &request_line, const ServerConfig &con);
private:
};

#endif //WEBSERV_CGI_HPP
