//
// Created by Knot Fonda on 7/10/21.
//

#ifndef WEBSERV_CGI_HPP_
#define WEBSERV_CGI_HPP_

#include "allowed_library_includes.hpp"
//#include "Server.hpp"
#include "parser.hpp"
#include <sstream>
#include "unistd.h"
#include "fcntl.h"
#include "Response.hpp"
// Сервер отправляет данные программе CGI с помощью переменных среды

// Задача:
// Сформировать execve->env
// При использовании метода GET данные отправляются в переменную QUERY_STRING
// При использовании метода GET данные отправляются в body (CONTENT_LENGTH)
//
//канонические переменные:
//AUTH_TYPE
//CONTENT_LENGTH
//CONTENT_TYPE
//GATEWAY_INTERFACE
//PATH_INFO
//PATH_TRANSLATED
//QUERY_STRING
//REMOTE_ADDR
//REMOTE_IDENT
//REMOTE_USER
//REQUEST_METHOD
//REQUEST_URI
//SCRIPT_NAME
//SERVER_NAME
//SERVER_PORT
//SERVER_PROTOCOL
//SERVER_SOFTWARE
//HTTP_X_SECRET_HEADER_FOR_TEST = 1

class CGI {
public:
  CGI(Request &req, const ServerConfig &con, std::string &str, std::string & result_dst);
  ~CGI();
	void executeCGI(std::string & result_dst);
	void mapToCString(std::map<std::string, std::string> &tmpEnv);
	void setEnv(Request &req);
    std::string & translate_path(std::string & path);
//private:
	char **_env;
//	const Response &response_;
	std::string str;
	Request &request_;
	const ServerConfig &_con;
};

#endif //WEBSERV_CGI_HPP_
